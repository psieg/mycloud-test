
#define STR(s)	#s
#define STRX(s)	STR(s)
#define MC_VERSION_MAJOR	0
#define MC_VERSION_MINOR	2
#define MC_VERSION_REVISION	8

#define MC_VERSION_RC		MC_VERSION_MAJOR, MC_VERSION_MINOR, MC_VERSION_REVISION, 0
#define MC_VERSION_PLAIN	STRX(MC_VERSION_MAJOR) "." STRX(MC_VERSION_MINOR) "." STRX(MC_VERSION_REVISION)

#ifdef _DEBUG
#	define MC_VERSION			MC_VERSION_PLAIN##" (debug)"
#else
#	define MC_VERSION			MC_VERSION_PLAIN
#endif
