#include "common.h"
#include "ae.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //inet_addr

size_t write_all(SOCKET fd, const char *buf, size_t buf_len)
{
    if (fd <= 0){
        return -1;
    }
    int write_len = 0;
    while (write_len < buf_len)
    {
        int len = 0;
        len = send(fd, buf + write_len, buf_len - write_len, 0);
        if (len < 0)
        {
            //LOGD("ERROR send[%d]:%s %d", len, strerror(len), buf_len);
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN) /* EAGAIN : Resource temporarily unavailable*/
            {
#ifdef WIN32
                Sleep(0);
#else
                sched_yield();
#endif
                continue;
            }
            else
            {
                return write_len;
            }
        }
        else
        {
            write_len = write_len + len;
        }
    }
    return write_len;
}

size_t read_all(SOCKET fd, char *buf, size_t buf_len)
{
    size_t len = 0, l = 0;
    do{
        l = recv(fd, buf + len, buf_len - len, 0);
        if(l == 0){
            return 0;
        }
        else if(l == (size_t)(-1)){
            int error = errno;
            //LOGE("read: %s", strerror(errno));
            if( error == EINTR || error == EWOULDBLOCK) {
                continue;
            }
            else if(error == EAGAIN){
#ifdef WIN32
                Sleep(0);
#else 
                sched_yield();
#endif
                continue;
                //break;
            }
            else {
                return 0;
            }
        }
        len += l;
    }while(buf_len != len);
    return len;
}


/* Return the UNIX time in microseconds */
long long ustime(void) {
    struct timeval tv;
    long long ust;

    gettimeofday(&tv, NULL);
    ust = ((long long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust;
}

/* Return the UNIX time in milliseconds */
mstime_t mstime(void) {
    return ustime()/1000;
}

/* Remove the specified client from global lists where the client could
 * be referenced, not including the Pub/Sub channels.
 * This is used by freeClient() and replicationCacheMaster(). */
void unlinkFileEvent(aeEventLoop *loop, int fd) {

    /* Certain operations must be done only if the client has an active socket.
     * If the client was already unlinked or if it's a "fake client" the
     * fd is already set to -1. */
    if (fd != -1) {
        /* Unregister async I/O handlers and close the socket. */
        aeDeleteFileEvent(loop, fd, AE_READABLE);
        aeDeleteFileEvent(loop, fd, AE_WRITABLE);
        close(fd);
        fd = -1;
    }
}

