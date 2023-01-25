typedef enum {
	LIBERTY_SIGNAL_OK = 0, /* nothing to do          */
	LIBERTY_SIGNAL_TERM,   /* close liberty          */
	LIBERTY_SIGNAL_UPDATE, /* trigger an update call */
} LibertySignal;
