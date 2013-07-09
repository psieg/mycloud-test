#ifndef MC_TYPES_H
#define MC_TYPES_H
#include "mc.h"

using namespace std;

/* config */
#define MC_MAXTIMEDIFF	10
#define MC_NETTIMEOUT	30
#define MC_SENDBLOCKSIZE 1024*1024 // 1MB
#define MC_RECVBLOCKSIZE 1024*1024 // 1MB

#ifdef MC_OS_WIN
	typedef __int64 int64;
#else
#	include <inttypes.h>
	typedef int64_t int64;
#	include <cstdlib> //malloc
#endif
#include "openssl/evp.h"

#ifdef MC_QTCLIENT
/* Events sent to QtClient with the MC_NOTIFY macro, -Start have a string parameter */
#	define MC_NT_CONN		0
#	define MC_NT_SYNC		1
#	define MC_NT_UL			2
#	define MC_NT_DL			3
#	define MC_NT_FULLSYNC	4 //A little out of row, has no end
#	define MC_NT_ERROR		5
#	ifdef MC_IONOTIFY
#		define MC_NT_DB		6
#		define MC_NT_FS		7
#		define MC_NT_SRV	8
#	endif
#	define MC_NT_WALKTEST	9
/* conflict dialog filename maxlen */
#	define MC_QTCONFLICTMAXLEN	35
#endif

//what to add to conflict files to be solve manually
//should be on the server's ignore list!
#define MC_CONFLICTEXTENSION	".mc_conflict"

#define MC_OK					0	//implicit, cant change
#define MC_ERR_DB				1
#define MC_ERR_PROTOCOL			2
#define MC_ERR_BADQRY			3	//TODO: candidate for deletion, use SERVER?
#define MC_ERR_SERVER			5
#define MC_ERR_LOGIN			6
#define MC_ERR_TIMEDIFF			7
#define MC_ERR_IO				8
#define MC_ERR_NETWORK			9
#define MC_ERR_VERIFY			10
#define MC_ERR_NOT_CONFIGURED	11
#define MC_ERR_CRYPTO			12
#define MC_ERR_CRYPTOALERT		13 //crypt_finish_download: TAG mismatch after download = the server can't be trusted
//These aren't really errors
#define MC_ERR_ALTCODE			20 //srv_perform: Server replied with altcode rather than requiredcode
#define MC_ERR_AUTODECIDE_UP	21 //autodecide: automatic decision upload
#define MC_ERR_AUTODECIDE_DOWN	22 //autodecide: automatic decision download
#define MC_ERR_AUTODECIDE_SKIP	23 //autodecide: automatic decision skip
#define MC_ERR_AUTODECIDE_NONE	24 //autodecide: no automatic choice
#define MC_ERR_INCOMPLETE_SKIP	25 //download: file is incomplete_up, those files are not downloaded
#define MC_ERR_CONFLICT_SKIP	26 //conflicted: the conflict was skipped
#define MC_ERR_UNCLEAN_WALK		27 //walk: noncritical errors occured
#define MC_ERR_NOTFULLYSYNCED	28 //fullsync: Not all hashes match
#define MC_ERR_NOCHANGE			29 //srv_notifychange: server returned no change

#define MC_ERR_TERMINATING		100 //Asynchronous terminate request from GUI
//Hopefully dev only
#define MC_ERR_NOT_IMPLEMENTED	101
#define MC_IS_CRITICAL_ERR(rc)		(rc && (rc == MC_ERR_TERMINATING || rc == MC_ERR_NETWORK || rc == MC_ERR_LOGIN || rc == MC_ERR_BADQRY || rc == MC_ERR_PROTOCOL || rc == MC_ERR_CRYPTO || rc == MC_ERR_CRYPTOALERT))


#define MC_FID_NONE -1
#define MC_SID_NONE -1

/* used by db */
/* These structs defines the format of all tables used, each struct represents one row of the specific table 
 *  they are meant to be used when accessing the database, as mc_db will use them to write binary data
 *  (instead of converting stuff to char strings to use them in db_execstr).
 */
typedef struct _mc_status {
	bool locked;
	int action;
	string url;
	string uname;
	string passwd;
	bool acceptallcerts;
	int watchmode;
	int64 basedate;
	int64 updatecheck;
	string updateversion;
} mc_status;

typedef int MC_SYNCSTATUS;
#define MC_SYNCSTAT_UNKOWN		0
#define MC_SYNCSTAT_RUNNING		1
#define MC_SYNCSTAT_COMPLETED	2
#define MC_SYNCSTAT_SYNCED		3
#define MC_SYNCSTAT_UNAVAILABLE	4
#define MC_SYNCSTAT_FAILED		5
#define MC_SYNCSTAT_ABORTED		6
#define MC_SYNCSTAT_DISABLED	7

typedef int MC_FILESTATUS;
#define MC_FILESTAT_COMPLETE		0
#define MC_FILESTAT_DELETED			1
#define MC_FILESTAT_INCOMPLETE_UP	2	// for files only
#define MC_FILESTAT_INCOMPLETE_DOWN	3	// for files only

typedef int MC_FILTERTYPE;
#define MC_FILTERT_MATCH_NAME			0
#define MC_FILTERT_MATCH_EXTENSION		1
#define MC_FILTERT_MATCH_FULLNAME		2
#define MC_FILTERT_MATCH_PATH			3
#define MC_FILTERT_REGEX_NAME			10
#define MC_FILTERT_REGEX_EXTENSION		11
#define MC_FILTERT_REGEX_FULLNAME		12
#define MC_FILTERT_REGEX_PATH			13

typedef int MC_CONFLICTACTION;
#define MC_CONFLICTACT_UNKNOWN	0	//not decided yet
#define MC_CONFLICTACT_SKIP		1
#define MC_CONFLICTACT_KEEP		2
#define MC_CONFLICTACT_UP		3
#define MC_CONFLICTACT_DOWN		4

typedef int MC_CONFLICTRECOMMENDATION;
#define MC_CONFLICTREC_DONTKNOW	0
#define MC_CONFLICTREC_UP		1
#define MC_CONFLICTREC_DOWN		2

inline bool compare_hashstr(const string& a, const string& b){ return *((int*)a.c_str()) < *((int*)b.c_str()); }

typedef struct _mc_sync {
	int id;
	string name;
	int filterversion;
	bool crypted;
	unsigned char hash[16];
} mc_sync;
inline bool compare_mc_sync(mc_sync a, mc_sync b){ return a.id < b.id; }

/* mc_sync */
/* data of sync in db. includes local directory */
typedef struct _mc_sync_db {
	int id;
	int priority;
	string name;
	string path;
	int filterversion;
	bool crypted;
	MC_SYNCSTATUS status;
	int64 lastsync;
	unsigned char hash[16];
	unsigned char cryptkey[32];
} mc_sync_db;
inline bool compare_mc_sync_db(mc_sync_db a, mc_sync_db b){ return a.id < b.id; }
inline bool compare_mc_sync_db_prio(mc_sync_db a, mc_sync_db b){ return a.priority < b.priority; }

typedef struct _mc_filter {
	int id;
	int sid;
	bool files;
	bool directories;
	MC_FILTERTYPE type;
	string rule;
} mc_filter;


typedef struct _mc_sync_ctx {
	mc_sync_db *sync;
	list<mc_filter> *filter;
	MC_CONFLICTACTION dirconflictact;	// confict action for current directory
	bool rconflictact;					// wether dirconflictact is recursive
} mc_sync_ctx;
inline void init_sync_ctx(mc_sync_ctx *ctx, mc_sync_db *sync, list<mc_filter> *filter){ 
	ctx->dirconflictact = MC_CONFLICTACT_UNKNOWN; ctx->rconflictact = false; 
	ctx->sync = sync; ctx->filter = filter; };

/* helper */
// comparison, not case sensitive.
inline bool nocase_smaller (const std::string& first, const std::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])<tolower(second[i])) return true;
    else if (tolower(first[i])>tolower(second[i])) return false;
    ++i;
  }
  if (first.length()<second.length()) return true;
  else return false;
}
inline bool nocase_greater (const std::string& first, const std::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])>tolower(second[i])) return true;
    else if (tolower(first[i])<tolower(second[i])) return false;
    ++i;
  }
  if (first.length()>second.length()) return true;
  else return false;
}
inline bool nocase_equals (const std::string& first, const std::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])>tolower(second[i])) return false;
    else if (tolower(first[i])<tolower(second[i])) return false;
    ++i;
  }
  if (first.length()==second.length()) return true;
  else return false;
}

typedef struct _mc_file {
	int id;
	string name;
	string cryptname; // = "" for unencrypted
	int64 ctime;
	int64 mtime;
	int64 size;
	bool is_dir;
	int parent;
	MC_FILESTATUS status;
	unsigned char hash[16];
} mc_file;
inline bool compare_mc_file(mc_file a, mc_file b){ return nocase_smaller(a.name,b.name); }
inline bool compare_mc_file_id(mc_file a, mc_file b){ return a.id < b.id; }
//DRAFT for walk_nochange to avoid side effects
//inline mc_file fakesrv(mc_file* db){ mc_file f; memcpy(&f,db,sizeof(mc_file)); f.name = std::string(db->name); return f; }

/* used by fs */
/* smaller mc_file with information available directly from the os (= without md5 hashes) */
typedef struct _mc_file_fs {
	string name;
	int64 ctime;
	int64 mtime;
	int64 size;
	bool is_dir;
} mc_file_fs;
inline bool compare_mc_file_fs(mc_file_fs a, mc_file_fs b){ return nocase_smaller(a.name,b.name); }

inline bool operator< (const string &a, const string &b){ return nocase_smaller(a,b); }
inline bool operator> (const string &a, const string &b){ return nocase_greater(a,b); }
inline bool operator== (const string &a, const string &b){ return nocase_equals(a,b); }

/* used by srv */
typedef struct _mc_buf {
  char *mem;
  size_t size;
  size_t used;
} mc_buf;

inline void SetBuf(mc_buf *buf){
	buf->mem = (char*) malloc(4);
	MC_MEM(buf->mem,4);
	buf->used = 0;
	buf->size = 4;
}
inline void SetBuf(mc_buf *buf, size_t size){
	buf->mem = (char*) malloc(size);
	MC_MEM(buf->mem,size);
	buf->used = 0;
	buf->size = size;
}
 
inline void ClearBuf(mc_buf *buf){
	buf->used = 0;
}

inline void ResetBuf(mc_buf *buf){
	void* p = realloc(buf->mem,4);
	MC_MEM(p,4);
	buf->mem = ( char*) p;
	buf->used = 0;
	buf->size = 4;
}

inline void MatchBuf(mc_buf *buf, size_t required){
	size_t newsize = buf->size;
	if(required > buf->size){
		if(newsize == 0) return; //error condition
		while(newsize < required) newsize *= 2;
		void* p = realloc(buf->mem,newsize);
		MC_MEM(p,newsize);
		buf->mem = (char*) p;
		MC_MEM(buf->mem,newsize);
		buf->size = newsize;
		//memset(&buf->mem[buf->used],0,required-buf->used);
	}
}

inline void FreeBuf(mc_buf *buf){
	free(buf->mem);
	buf->size = 0;
	buf->used = 0;
	buf->mem = NULL;
}


/* used for crypt */
typedef struct _mc_crypt_ctx {
	mc_sync_ctx *ctx;
	EVP_CIPHER_CTX *evp;
	mc_file *f;
	unsigned char iv[16];
	bool hasiv;
	unsigned char tag[16];
	bool hastag;
	mc_buf pbuf; //persistent buffer to avoid mallocs
} mc_crypt_ctx;
inline void init_crypt_ctx(mc_crypt_ctx *cctx, mc_sync_ctx *ctx)
	{ cctx->hasiv = false; cctx->hastag = false; cctx->f = NULL; cctx->ctx = ctx; }
inline void init_crypt_ctx_copy(mc_crypt_ctx *cctx, mc_crypt_ctx *extcctx)
	{ cctx->hasiv = extcctx->hasiv; cctx->hastag = extcctx->hastag; cctx->f = extcctx->f; cctx->ctx = extcctx->ctx; }



#endif /* MC_TYPES_H */