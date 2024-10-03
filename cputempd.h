
#define FD_NUM_REASONABLE   1024
#define FD_STDOUT           0
#define FD_STDIN            1
#define FD_STDERR           2

#define CPU_SEARCH_0        "Tccd"
#define CPU_SEARCH_LEN_0    4

#define CPU_SEARCH_1        "Package"
#define CPU_SEARCH_LEN_1    7

#define CPU_SEARCH_FAIL     1
#define CPU_SEARCH_SUCCESS  0
#define CPU_SEARCH_STOP     0
#define CPU_SEARCH_GO       1
#define CPU_TEMP_FEAT       SENSORS_FEATURE_TEMP
#define CPU_TEMP_SUBFEAT    SENSORS_SUBFEATURE_TEMP_INPUT

#define FIFO_PATH           "/var/run/cputempd.fifo"
#define FIFO_CLIENT_DIR     "/tmp/cputempd"
#define FIFO_CLIENT_PATH    "/tmp/cputempd/cputempd."

#define TEMP_STR_LEN        8

