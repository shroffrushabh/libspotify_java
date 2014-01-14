#include <stdio.h>
#include <stdlib.h>

#include <libspotify/api.h>

/* --- Data --- */
/// The application key is specific to each project, and allows Spotify
/// to produce statistics on how our service is used.
extern const uint8_t g_appkey[];
/// The size of the application key.
extern const size_t g_appkey_size;
int g_logged_in;

/// Non-zero when a track has ended and the jukebox has not yet started a new one
static int g_playback_done;
/// The global session handle
static sp_session *g_sess;
/// Handle to the playlist currently being played
static sp_playlist *g_jukeboxlist;
/// Name of the playlist currently being played
const char *g_listname;
/// Remove tracks flag
static int g_remove_tracks = 0;
/// Handle to the curren track
static sp_track *g_currenttrack;
/// Index to the next track
static int g_track_index;

/* ---------------------------  SESSION CALLBACKS  ------------------------- */
/**
 * This callback is called when an attempt to login has succeeded or failed.
 *
 * @sa sp_session_callbacks#logged_in
 */
static void logged_in(sp_session *sess, sp_error error)
{
    if (error != SP_ERROR_OK) {
        g_logged_in = 2;
    }
    else {
    g_logged_in = 1;
    }
}

/**
 * The session callbacks
 */
static sp_session_callbacks session_callbacks = {
    .logged_in = &logged_in
};

/**
 * The session configuration. Note that application_key_size is an external, so
 * we set it in main() instead.
 */
static sp_session_config spconfig = {
	.api_version = SPOTIFY_API_VERSION,
	.cache_location = "tmp",
	.settings_location = "tmp",
	.application_key = g_appkey,
	.application_key_size = 0, // Set in main()
	.user_agent = "login-plugin",
	.callbacks = &session_callbacks,
	NULL,
};
/* -------------------------  END SESSION CALLBACKS  ----------------------- */


static int login(char *user, char *pass)
{
    sp_session *sp;
	sp_error err;
    
    /* Create session */
	spconfig.application_key_size = g_appkey_size;
    
	err = sp_session_create(&spconfig, &sp);
    
	if (SP_ERROR_OK != err) {
		fprintf(stderr, "Unable to create session: %s\n",
                sp_error_message(err));
        g_logged_in = 3;
	}
    
	g_sess = sp;

	int next_timeout = 0;
	sp_session_login(sp, user, pass, 0, NULL);

    for (;;) {
        sp_session_process_events(sp, &next_timeout);
        if (g_logged_in != 0) {
            return g_logged_in;
        }
    }
}

/**
 * Show usage information
 *
 * @param  progname  The program name
 */
static void usage(const char *progname)
{
	fprintf(stderr, "usage: %s -u <username> -p <password> -l <listname> [-d]\n", progname);
}

int main()
{
	//int next_timeout = 0;
	const char *username = NULL;
	const char *password = NULL;
	int opt;

	/*while ((opt = getopt(argc, argv, "u:p:l:d")) != EOF) {
		switch (opt) {
		case 'u':
			username = optarg;
			break;

     case 'p':
			password = optarg;
			break;

		default:
			exit(1);
		}
	}*/
    printf("%d - value\n",login(username,password));
    
	return 0;
}
