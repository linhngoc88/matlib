#if defined(INS_USE_NUMERIC_TYPE_DOUBLE)
#define INS_NUMERIC_TYPE double
#define INS_NUMERIC_INPUT_FORMAT "%lg"
#define INS_NUMERIC_OUT_FORMAT "%g"
#define INS_NUMERIC_ZERO 0.0
#define INS_NUMERIC_ONE 1.0
#else
#error Unkown INS_USE_NUMERIC_TYPE_ DIRECTIVE
#endif

#define CONCAT2x(a,b) a ## _ ## b
#define CONCAT2(a,b) CONCAT2x(a,b)
#define CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define CONCAT3(a,b,c) CONCAT3x(a,b,c)

#if defined(INS_USE_NUMERIC_TYPE_DOUBLE)
#define INS_FUNC(dir,name) CONCAT2(dir,name)
#define INS_TYPE(dir) dir
#else
#define INS_FUNC(dir,name) CONCAT3(dir,INS_NUMERIC_TYPE,name)
#define INS_TYPE(dir) CONCAT2(dir,INS_NUMERIC_TYPE)
#endif
