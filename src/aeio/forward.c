#include "forward.h"
#include "common.h"
#define BUFFER_SIZE (1024 * 30)

static int copy_socket(int fd1, int fd2, void* buf, int size)
{
    int rlen = 0, wlen = 0, len = 0;
    rlen = recv(fd1, buf, size, 0);
    if(rlen <= 0){
        //LOGD("read failed %d", rc);
        return rlen;    
    }
    do{
        len = send(fd2, buf + wlen, rlen - wlen, 0);
        if(len <= 0){
            //LOGD("write failed %d", rc);
            return len; 
        }
        wlen += len;
    }while(wlen != rlen);

    return wlen;
}
