/*
 *
 * (C) 2017 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lessed General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>

#include <libssh/libssh.h>

#ifndef DLT_EN10MB
#define DLT_EN10MB	1
#endif

#define REMOTENTOPDUMP_TIMELINE "remote-n2disk-timeline"
#define REMOTENTOPDUMP_MAX_DATE_LEN     26
#define REMOTENTOPDUMP_MAX_PATH_LEN     256
#define REMOTENTOPDUMP_SSH_BLOCK_SIZE   1024
#define REMOTENTOPDUMP_VERSION_MAJOR    "0"
#define REMOTENTOPDUMP_VERSION_MINOR    "1"
#define REMOTENTOPDUMP_VERSION_RELEASE  "0"

#define EXTCAP_OPT_LIST_INTERFACES		'l'
#define EXTCAP_OPT_VERSION			'v'
#define EXTCAP_OPT_LIST_DLTS			'L'
#define EXTCAP_OPT_INTERFACE			'i'
#define EXTCAP_OPT_CONFIG			'c'
#define EXTCAP_OPT_CAPTURE			'C'
#define EXTCAP_OPT_CAPTURE_FILTER		'f'
#define EXTCAP_OPT_FIFO				'F'
#define EXTCAP_OPT_DEBUG			'D'
#define REMOTENTOPDUMP_OPT_HELP			'h'
#define REMOTENTOPDUMP_OPT_PATH			't'
#define REMOTENTOPDUMP_OPT_START_TIME		's'
#define REMOTENTOPDUMP_OPT_END_TIME		'e'
#define REMOTENTOPDUMP_OPT_START_TIME_EPOCH	'S'
#define REMOTENTOPDUMP_OPT_END_TIME_EPOCH	'E'
#define REMOTENTOPDUMP_OPT_NDPI			'd'
#define REMOTENTOPDUMP_OPT_SSH_HOST             'H'
#define REMOTENTOPDUMP_OPT_SSH_PORT             'P'
#define REMOTENTOPDUMP_OPT_SSH_USERNAME         'U'
#define REMOTENTOPDUMP_OPT_SSH_PASSWORD         'W'
#define REMOTENTOPDUMP_OPT_SSH_KEY              'K'
#define REMOTENTOPDUMP_OPT_SSH_KEY_PASSPHRASE   'R'

static struct option longopts[] = {
  /* mandatory extcap options */
  { "extcap-interfaces",	no_argument, 		NULL, EXTCAP_OPT_LIST_INTERFACES },
  { "extcap-version", 		optional_argument, 	NULL, EXTCAP_OPT_VERSION },
  { "extcap-dlts", 		no_argument, 		NULL, EXTCAP_OPT_LIST_DLTS },
  { "extcap-interface", 	required_argument, 	NULL, EXTCAP_OPT_INTERFACE },
  { "extcap-config", 		no_argument, 		NULL, EXTCAP_OPT_CONFIG },
  { "capture", 			no_argument, 		NULL, EXTCAP_OPT_CAPTURE },
  { "extcap-capture-filter", 	required_argument,	NULL, EXTCAP_OPT_CAPTURE_FILTER },
  { "fifo", 			required_argument, 	NULL, EXTCAP_OPT_FIFO },
  { "debug", 			optional_argument, 	NULL, EXTCAP_OPT_DEBUG },

  /* custom extcap options */
  { "help", 			no_argument, 		NULL, REMOTENTOPDUMP_OPT_HELP },
  { "timeline-path", 		required_argument,	NULL, REMOTENTOPDUMP_OPT_PATH },
  { "start", 			required_argument, 	NULL, REMOTENTOPDUMP_OPT_START_TIME },
  { "end", 			required_argument, 	NULL, REMOTENTOPDUMP_OPT_END_TIME },
  { "start-epoch", 		required_argument, 	NULL, REMOTENTOPDUMP_OPT_START_TIME_EPOCH },
  { "end-epoch", 		required_argument, 	NULL, REMOTENTOPDUMP_OPT_END_TIME_EPOCH },
  { "ndpi", 			no_argument, 		NULL, REMOTENTOPDUMP_OPT_NDPI },
  { "ssh-host",                 required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_HOST },
  { "ssh-port",                 required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_PORT },
  { "ssh-username",             required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_USERNAME },
  { "ssh-password",             required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_PASSWORD },
  { "ssh-key",                  required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_KEY },
  { "ssh-key-passphrase",       required_argument,      NULL, REMOTENTOPDUMP_OPT_SSH_KEY_PASSPHRASE },

  {0, 0, 0, 0}
};

typedef struct _extcap_interface {
  char *interface;
  char *description;

  u_int16_t dlt;
  char *dltname;
  char *dltdescription;
} extcap_interface;

static extcap_interface extcap_interfaces[] = {
  { REMOTENTOPDUMP_TIMELINE,  "Remote n2disk timeline" , DLT_EN10MB, NULL, "The EN10MB Ethernet2 DLT" }
};
static size_t extcap_interfaces_num = sizeof(extcap_interfaces) / sizeof(extcap_interface);

static char *extcap_selected_interface   = NULL;
static char *extcap_capture_filter       = NULL;
static char *extcap_capture_fifo         = NULL;
static char *ntopdump_path               = NULL;
static char *ntopdump_start              = NULL;
static char *ntopdump_end                = NULL;
static int ntopdump_ndpi                 = 0;
static char *ntopdump_ssh_host           = NULL;
static int ntopdump_ssh_port             = 0;
static char *ntopdump_ssh_username       = NULL;
static char *ntopdump_ssh_password       = NULL;
static char *ntopdump_ssh_key            = NULL;
static char *ntopdump_ssh_key_passphrase = NULL;

static void close_ssh_connection(ssh_session session, ssh_channel channel) {
  if (channel != NULL) {
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
  }

  if (session != NULL) {
    ssh_disconnect(session);
    ssh_free(session);
    session = NULL;
  }
}

static ssh_channel setup_ssh_channel(ssh_session session) {
  ssh_channel channel;

  channel = ssh_channel_new(session);

  if (!channel) {
    fprintf(stderr, "Error creating channel\n");
    return NULL;
  }

  if (ssh_channel_open_session(channel) != SSH_OK) {
    fprintf(stderr, "Error opening session\n");
    ssh_channel_free(channel);
    return NULL;
  }

  return channel;
}

static ssh_session setup_ssh_connection() {
  ssh_session session;
  int rc;

  session = ssh_new();

  if (session == NULL) {
    fprintf(stderr, "Error creating ssh session\n");
    return NULL;
  }

  if (ssh_options_set(session, SSH_OPTIONS_HOST, ntopdump_ssh_host)) {
    fprintf(stderr, "Error setting host %s\n", ntopdump_ssh_host);
    goto error;
  }

  if (ntopdump_ssh_port != 0) {
    if (ssh_options_set(session, SSH_OPTIONS_PORT, &ntopdump_ssh_port)) {
      fprintf(stderr, "Error setting port %d\n", ntopdump_ssh_port);
      goto error;
    }
  }

  if (ssh_options_set(session, SSH_OPTIONS_USER, ntopdump_ssh_username)) {
    fprintf(stderr, "Error setting username %s\n", ntopdump_ssh_username);
    goto error;
  }

  printf("Connecting to %s@%s:%u\n", ntopdump_ssh_username, ntopdump_ssh_host, ntopdump_ssh_port);

  if (ssh_connect(session) != SSH_OK) {
    fprintf(stderr, "Error connecting to %s@%s:%u (%s)\n", ntopdump_ssh_username, ntopdump_ssh_host, ntopdump_ssh_port, ssh_get_error(session));
    goto error;
  }

  if (ntopdump_ssh_key != NULL) {
    ssh_key pkey = ssh_key_new();

    printf("Connecting using public key in %s\n", ntopdump_ssh_key);

    rc = ssh_pki_import_privkey_file(ntopdump_ssh_key, ntopdump_ssh_key_passphrase, NULL, NULL, &pkey);

    if (rc == SSH_OK) {
      if (ssh_userauth_publickey(session, NULL, pkey) == SSH_AUTH_SUCCESS) {
        ssh_key_free(pkey);
        return session;
      }
    }
    ssh_key_free(pkey);
  }

  printf("Connecting using public key\n");

  if (ssh_userauth_publickey_auto(session, ntopdump_ssh_username, ntopdump_ssh_key_passphrase) == SSH_AUTH_SUCCESS)
    return session;

  if (ntopdump_ssh_password != NULL) {

    printf("Connecting using password\n");

    if (ssh_userauth_password(session, ntopdump_ssh_username, ntopdump_ssh_password) == SSH_AUTH_SUCCESS)
      return session;
  }

  fprintf(stderr, "Unable to authenticate\n");

  ssh_disconnect(session);

error:
  ssh_free(session);

  return NULL;
}

void sigproc(int sig) {
  fprintf(stdout, "Exiting...\n");
  fflush(stdout);
}

void extcap_version() {
  printf("extcap {version=%s.%s.%s}\n", REMOTENTOPDUMP_VERSION_MAJOR, REMOTENTOPDUMP_VERSION_MINOR, REMOTENTOPDUMP_VERSION_RELEASE);
}

void extcap_list_interfaces() {
  int i;
  
  for(i = 0; i < extcap_interfaces_num; i++) {
    printf("interface {value=%s}{display=%s}\n", extcap_interfaces[i].interface, extcap_interfaces[i].description);    
  }
}

void extcap_dlts() {
  int i;
  
  if(!extcap_selected_interface) return;
  for(i = 0; i < extcap_interfaces_num; i++) {
    extcap_interface *eif = &extcap_interfaces[i];
    if(!strncmp(extcap_selected_interface, eif->interface, strlen(eif->interface))) {
      printf("dlt {number=%u}{name=%s}{display=%s}\n", eif->dlt, eif->interface, eif->dltdescription);
      break;
    }
  }
}

int exec_head(const char *bin, char *line, size_t line_len) {
  FILE *fp;

  fp = popen(bin, "r");

  if (fp == NULL)
    return -1;

  if (fgets(line, line_len-1, fp) == NULL) {
    pclose(fp);
    return -1;
  }

  pclose(fp);
  return 0;
}

float wireshark_version() {
  float v = 0;
  char line[1035];
  char *version, *rev;

  if (exec_head("/usr/bin/wireshark -v", line, sizeof(line)) != 0 &&
      exec_head("/usr/local/bin/wireshark -v", line, sizeof(line)) != 0)
      return 0;

  version = strchr(line, ' ');
  if (version == NULL) return 0;
  version++;
  rev = strchr(version, '.');
  if (rev == NULL) return 0;
  rev++;
  rev = strchr(rev, '.');
  if (rev == NULL) return 0;
  *rev = '\0';

  sscanf(version, "%f", &v);

  return v;
}

void extcap_config() {
  u_int argidx = 0;

  if(!extcap_selected_interface) return;

  if (!strncmp(extcap_selected_interface, REMOTENTOPDUMP_TIMELINE, strlen(REMOTENTOPDUMP_TIMELINE))) {
    time_t timer;
    char time_buffer_start[REMOTENTOPDUMP_MAX_DATE_LEN], time_buffer_end[REMOTENTOPDUMP_MAX_DATE_LEN];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);
    strftime(time_buffer_end, REMOTENTOPDUMP_MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
    timer -= 5 * 60;
    tm_info = localtime(&timer);
    strftime(time_buffer_start, REMOTENTOPDUMP_MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("arg {number=%u}{call=--timeline-path}"
	   "{display=n2disk timeline path}{type=string}{default=/storage}"
	   "{tooltip=The n2disk timeline path (e.g. /storage/n2disk/eth1/timeline)}\n", argidx++);

#ifndef __APPLE__
    if (wireshark_version() > 2.2) {
      printf("arg {number=%u}{call=--start-epoch}"
	     "{display=Start date and time}{type=timestamp}"
	     "{tooltip=The start of the extraction interval}\n", argidx++);
      printf("arg {number=%u}{call=--end-epoch}"
	     "{display=End date and time}{type=timestamp}"
	     "{tooltip=The end of the extraction interval}\n", argidx++);
    } else {
#endif
      printf("arg {number=%u}{call=--start}"
	     "{display=Start date and time}{type=string}{default=%s}"
	     "{tooltip=The start of the extraction interval (e.g., %s)}\n", argidx++, time_buffer_start, time_buffer_start);
      printf("arg {number=%u}{call=--end}"
	     "{display=End date and time}{type=string}{default=%s}"
	     "{tooltip=The end of the extraction interval (e.g., %s)}\n", argidx++, time_buffer_end, time_buffer_end);
#ifndef __APPLE__
    }
#endif

    printf("arg {number=%u}{call=--ndpi}"
	   "{display=enable nDPI inspection}{type=boolflag}{default=true}"
	   "{tooltip=Enable nDPI inspection to provide L7 informations}\n", argidx++);

    printf("arg {number=%u}{call=--ssh-host}{display=Remote SSH server address}"
           "{type=string}{tooltip=The remote SSH host (IP address or hostname)}{required=true}\n", argidx++);
    printf("arg {number=%u}{call=--ssh-port}{display=Remote SSH server port}"
           "{type=unsigned}{default=22}{tooltip=The remote SSH host port (1-65535)}"
           "{range=1,65535}\n", argidx++);
    printf("arg {number=%u}{call=--ssh-username}{display=Remote SSH server username}"
           "{type=string}{default=root}{tooltip=The remote SSH username}\n", argidx++);
    printf("arg {number=%u}{call=--ssh-password}{display=Remote SSH server password}"
           "{type=password}{tooltip=The SSH password (used when SSH key in not available, empty to auto detect public key)}\n", argidx++);
    printf("arg {number=%u}{call=--ssh-key}{display=Path to SSH private key}"
           "{type=fileselect}{tooltip=The path on the local filesystem of the private ssh key, empty to auto detect public key}\n", argidx++);
    printf("arg {number=%u}{call=--ssh-key-passphrase}{display=SSH key passphrase}"
           "{type=password}{tooltip=Passphrase to unlock the SSH private or public key}\n", argidx++);
  }
}

void extcap_capture() {
  char buffer[REMOTENTOPDUMP_SSH_BLOCK_SIZE];
  char *command;
  ssh_session session = NULL;
  ssh_channel channel = NULL;
  FILE *fp = NULL;
  int nbytes;
  int command_len;

  if (ntopdump_path == NULL) {
    fprintf(stderr, "Timeline path needed\n");
    return;
  }

  if (ntopdump_start == NULL) {
    fprintf(stderr, "Start time needed\n");
    return;
  }

  if (ntopdump_end == NULL) {
    fprintf(stderr, "End time needed\n");
    return;
  }

  if (ntopdump_ssh_host == NULL) {
    fprintf(stderr, "Hostname needed\n");
    return;
  }

  if (ntopdump_ssh_username == NULL) {
    fprintf(stderr, "Username needed\n");
    return;
  }

  if (extcap_capture_filter == NULL)
    extcap_capture_filter = strdup("");

  command_len = REMOTENTOPDUMP_MAX_PATH_LEN + 2 * strlen("2017-05-23 17:00:00") + strlen(extcap_capture_filter) + 100; 
  command = (char *) calloc(command_len, sizeof(char)); 

  if (command == NULL) {
    fprintf(stderr, "Failure allocating memory\n");
    return;
  }

  if (extcap_capture_fifo != NULL && strcmp(extcap_capture_fifo, "-") != 0) {
    fp = fopen(extcap_capture_fifo, "wb");

    if (fp == NULL) {
      fprintf(stderr, "Error creating output file: %s (%s)\n", extcap_capture_fifo, strerror(errno));
      return;
    }
  }

  snprintf(command, command_len, "npcapextract -t %s -b \"%s\" -e \"%s\" -f \"%s\" -O %s",
    ntopdump_path, ntopdump_start, ntopdump_end, extcap_capture_filter, ntopdump_ndpi ? "| ndpiReader -i - --capture --fifo -" : "");

  session = setup_ssh_connection();

  if (session == NULL) {
    fprintf(stderr, "Error creating connection\n");
    goto exit;
  }

  channel = setup_ssh_channel(session);

  if (channel == NULL) {
    fprintf(stderr, "Error creating channel\n");
    goto exit;
  }

  printf("Running: %s\n", command);

  if (ssh_channel_request_exec(channel, command) != SSH_OK) {
    fprintf(stderr, "Can't request exec\n");
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    channel = NULL;
  }

  while (ssh_channel_is_open(channel) && !ssh_channel_is_eof(channel)) {
    nbytes = ssh_channel_read(channel, buffer, REMOTENTOPDUMP_SSH_BLOCK_SIZE, 0);
    if (nbytes <= 0) {
      if (nbytes < 0)
        fprintf(stderr, "Error reading from channel\n");
      goto exit;
    }
    if (fwrite(buffer, 1, nbytes, fp != NULL ? fp : stdout) != (u_int) nbytes) {
      fprintf(stderr, "Error writing to fifo\n");
      goto exit;
    }
    fflush(fp != NULL ? fp : stdout);
  }

exit:
  close_ssh_connection(session, channel);

  if (fp != NULL)
    fclose(fp);
}

int extcap_print_help() {
  printf("Wireshark extcap plugin by ntop\n");
  printf("Supported interfaces:\n");
  extcap_list_interfaces();
  return 0;
}

int main(int argc, char *argv[]) {
  int option_idx = 0, result;
  time_t epoch;
  char date_str[REMOTENTOPDUMP_MAX_DATE_LEN];
  struct tm* tm_info;

  if (argc == 1) {
    extcap_print_help();
    return EXIT_SUCCESS;
  }

  u_int defer_dlts = 0, defer_config = 0, defer_capture = 0;
  while ((result = getopt_long(argc, argv, "h", longopts, &option_idx)) != -1) {
    switch (result) {
    /* mandatory extcap options */
    case EXTCAP_OPT_DEBUG:
      break;
    case EXTCAP_OPT_LIST_INTERFACES:
      extcap_version();
      extcap_list_interfaces();
      defer_dlts = defer_config = defer_capture = 0;
      break;
    case EXTCAP_OPT_VERSION:
      extcap_version();
      defer_dlts = defer_config = defer_capture = 0;
      break;
    case EXTCAP_OPT_LIST_DLTS:
      defer_dlts = 1; defer_config = defer_capture = 0;
      break;
    case EXTCAP_OPT_INTERFACE:
      extcap_selected_interface = strdup(optarg);
      break;
    case EXTCAP_OPT_CONFIG:
      defer_config = 1; defer_dlts = defer_capture = 0;
      break;
    case EXTCAP_OPT_CAPTURE:
      defer_capture = 1; defer_dlts = defer_config = 0;
      break;
    case EXTCAP_OPT_CAPTURE_FILTER:
      extcap_capture_filter = strdup(optarg);
      break;
    case EXTCAP_OPT_FIFO:
      extcap_capture_fifo = strdup(optarg);
      break;

    /* custom ntopdump options */
    case REMOTENTOPDUMP_OPT_HELP:
      extcap_print_help();
      return EXIT_SUCCESS;
      break;
    case REMOTENTOPDUMP_OPT_PATH:
      if (ntopdump_path == NULL)
        ntopdump_path = strdup(optarg);
      break;
    case REMOTENTOPDUMP_OPT_START_TIME:
      if (ntopdump_start == NULL)
        ntopdump_start = strndup(optarg, REMOTENTOPDUMP_MAX_DATE_LEN);
      break;
    case REMOTENTOPDUMP_OPT_END_TIME:
      if (ntopdump_end == NULL)
        ntopdump_end = strndup(optarg, REMOTENTOPDUMP_MAX_DATE_LEN);
      break;
    case REMOTENTOPDUMP_OPT_START_TIME_EPOCH:
      if (ntopdump_start != NULL) free(ntopdump_start);
      epoch = atoi(optarg);
      tm_info = localtime(&epoch);
      strftime(date_str, REMOTENTOPDUMP_MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
      ntopdump_start = strndup(date_str, REMOTENTOPDUMP_MAX_DATE_LEN);
      break;
    case REMOTENTOPDUMP_OPT_END_TIME_EPOCH:
      if (ntopdump_end != NULL) free(ntopdump_end);
      epoch = atoi(optarg);
      tm_info = localtime(&epoch);
      strftime(date_str, REMOTENTOPDUMP_MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", tm_info);
      ntopdump_end = strndup(date_str, REMOTENTOPDUMP_MAX_DATE_LEN);
      break;
    case REMOTENTOPDUMP_OPT_NDPI:
      ntopdump_ndpi = 1;
      break;
    case REMOTENTOPDUMP_OPT_SSH_HOST:
      if (ntopdump_ssh_host == NULL)
        ntopdump_ssh_host = strdup(optarg);
      break;
    case REMOTENTOPDUMP_OPT_SSH_PORT:
      ntopdump_ssh_port = atoi(optarg);
      break;
    case REMOTENTOPDUMP_OPT_SSH_USERNAME:
      if (ntopdump_ssh_username == NULL)
        ntopdump_ssh_username = strdup(optarg);
      break;
    case REMOTENTOPDUMP_OPT_SSH_PASSWORD:
      if (ntopdump_ssh_password == NULL)
        ntopdump_ssh_password = strdup(optarg);
      break;
    case REMOTENTOPDUMP_OPT_SSH_KEY:
      if (ntopdump_ssh_key == NULL)
        ntopdump_ssh_key = strdup(optarg);
      break;
    case REMOTENTOPDUMP_OPT_SSH_KEY_PASSPHRASE:
      if (ntopdump_ssh_key_passphrase == NULL)
        ntopdump_ssh_key_passphrase = strdup(optarg);
      break;
    }
  }

  signal(SIGINT, sigproc);
  signal(SIGTERM, sigproc);
  signal(SIGQUIT, sigproc);

  if(defer_dlts) extcap_dlts();
  else if(defer_config) extcap_config();
  else if(defer_capture && ntopdump_path) extcap_capture();

  if(extcap_selected_interface)   free(extcap_selected_interface);
  if(extcap_capture_filter)       free(extcap_capture_filter);
  if(extcap_capture_fifo)         free(extcap_capture_fifo);
  if(ntopdump_path)               free(ntopdump_path);
  if(ntopdump_start)              free(ntopdump_start);
  if(ntopdump_end)                free(ntopdump_end);
  if(ntopdump_ssh_host)           free(ntopdump_ssh_host);
  if(ntopdump_ssh_username)       free(ntopdump_ssh_username);
  if(ntopdump_ssh_password)       free(ntopdump_ssh_password);
  if(ntopdump_ssh_key)            free(ntopdump_ssh_key);
  if(ntopdump_ssh_key_passphrase) free(ntopdump_ssh_key_passphrase);

  return EXIT_SUCCESS;
}
