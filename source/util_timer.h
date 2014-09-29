#ifdef _WIN32 // MSC_VER
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h> // Windows.h -> WinDef.h defines min() max()

    /*
        typedef uint16_t WORD ;
        typedef uint32_t DWORD;

        typedef struct _FILETIME {
            DWORD dwLowDateTime;
            DWORD dwHighDateTime;
        } FILETIME;

        typedef struct _SYSTEMTIME {
              WORD wYear;
              WORD wMonth;
              WORD wDayOfWeek;
              WORD wDay;
              WORD wHour;
              WORD wMinute;
              WORD wSecond;
              WORD wMilliseconds;
        } SYSTEMTIME, *PSYSTEMTIME;
    */

    // WTF!?!? Exists in winsock2.h
    typedef struct timeval {
        long tv_sec;
        long tv_usec;
    } timeval;

    // *sigh* no gettimeofday on Win32/Win64
    int gettimeofday(struct timeval * tp, struct timezone * tzp)
    {
        // FILETIME Jan 1 1970 00:00:00
        // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
        static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL); 

        SYSTEMTIME  system_time;
        FILETIME    file_time;
        uint64_t    time;

        GetSystemTime( &system_time );
        SystemTimeToFileTime( &system_time, &file_time );
        time =  ((uint64_t)file_time.dwLowDateTime )      ;
        time += ((uint64_t)file_time.dwHighDateTime) << 32;

        tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
        tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
        return 0;
    }
#endif // WIN32

struct DataRate
{
    char     prefix ;
    uint64_t samples;
    uint64_t per_sec;
};

class Timer
{
    timeval start, end; // Windows: winsock2.h  Unix: sys/time.h 
public:
    double   elapsed; // total seconds
    uint32_t mins;
    uint32_t secs;
    DataRate throughput;

    void Start() {
        gettimeofday( &start, NULL );
    }

    void Stop() {
        gettimeofday( &end, NULL );
        elapsed = (end.tv_sec - start.tv_sec);

        mins = (uint32_t)elapsed / 60;
        secs = (uint32_t)elapsed - (mins*60);
    }

    // size is number of bytes in a file, or number of iterations that you want to benchmark
    void Throughput( uint64_t size )
    {
        const int MAX_PREFIX = 4;
        DataRate datarate[ MAX_PREFIX ] = {
            {' '}, {'K'}, {'M'}, {'G'} // 1; 1,000; 1,000,000; 1,000,000,000
        };

        int best = 0;
        for( int units = 0; units < MAX_PREFIX; units++ ) {
            datarate[ units ].samples  = size >> (10*units);
            datarate[ units ].per_sec = (uint64_t) (datarate[units].samples / elapsed);
            if (datarate[units].per_sec > 0)
                best = units;
        }
        throughput = datarate[ best ];
    }
};
