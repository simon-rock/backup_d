#include <sys/types.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

//#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/mount.h>
#include <linux/nfs.h>
#include <linux/nfs_mount.h>
#include <sys/param.h>
#include <string.h>
#include <fstream>
using namespace std;
int main (int argc, char *argv[]) {

    if (argc > 1)
    {
        // umount
        if (umount("/home/yu/mount_test") == 0)
        {
            printf ("umount sucess\n");
        }
        else
        {
            printf ("umount failed\n");
        }
        return 0;
    }
struct nfs2_fh estruc1={""};
struct sockaddr_in estruc2; //={AF_INET,0};
//estruc2.sin_family = AF_INET;
//estruc2.sin_port = htons((unsigned short)atoi("9999"));
//estruc2.sin_addr.s_addr = inet_addr("192.168.1.50");
struct nfs3_fh estruc3={0,""};
struct nfs_mount_data data={6, /* version */
1, /* fd */
estruc1,
1, /* flags */
4096, /* rsize */
4096, /* wsize */
1000, /* timeo */
3, /* retrans */
3, /* acregmin */
60, /* acregmax */
30, /* acdirmin */
60, /* acdirmax */
estruc2,
"192.16.1.50", /* hostname */
12, /* namelen */
1, /* bsize */
estruc3,
4,
""};

//const char *remoto = "/mnt/brick/logs";
const char *remoto = "/dev/sda1";
const char *puntomontaje = "/home/yu/mount_test";
//const char *tipomontaje = "nfs";
const char *tipomontaje = "ext4";
unsigned long int flags = MS_MGC_VAL|MS_RDONLY;
int flags_umount = 0;

int error;

//if (mount(remoto, puntomontaje, tipomontaje, flags, &data) == 0) {
if (mount(remoto, puntomontaje, tipomontaje, flags, "") == 0) {
    printf("Se ha montado correctamente\n");
    ofstream o("/etc/mtab", ios::app);
    if (o)
    {
        // o << endl;
        o << remoto << " " << puntomontaje <<  " " << tipomontaje << " rw 0 0" << endl;
    }
} else {
printf("No se ha podido montar.");
printf("mensaje_error %d %s\n",errno, strerror(errno));
//switch(errno){
//case EBUSY: printf("Ya esta montado\n"); break;
//case EINVAL: printf("einval\n"); break;
//default: break;
//}
}
}
