#ifndef _IVC_ECHO_H_
#define _IVC_ECHO_H_

// unsigned int Buffer[524288]__attribute__((aligned(0x00200000)));		// 0x200000 B
#define Buffer 0x80200000

#define R_WRITE_COUNT     (*(volatile unsigned int*)(Buffer))      // freertos 总共发送过来的帧数           write_header.write.d
#define A_READ_COUNT      (*(volatile unsigned int*)(Buffer + 0x40))      // A核总共读取的帧数（注意：读完更新）     write_header.read.d
#define R_STATE           (*(volatile unsigned int*)(Buffer + 0x04))
#define WRITE_BUFFER_BASE (Buffer + 0x80)            // write_Buffer起始地址（读的基地址）

#define A_WRITE_COUNT     (*(volatile unsigned int*)(Buffer + 0x10000))      // A核总共发送的帧数（注意：写完更新）     read_header.write.d
#define R_READ_COUNT      (*(volatile unsigned int*)(Buffer + 0x10040))      // freertos 总共读取的帧数               read_header.read.d
#define A_STATE           (*(volatile unsigned int*)(Buffer + 0x10004))
#define READ_BUFFER_BASE  (Buffer + 0x10080)              // read_Buffer起始地址（写的基地址）

#define nframes             16
#define frame_size          64

#define ivc_carveout_base_ss Buffer
#define ivc_carveout_size_ss 0x00200000

void msg_rx();
void msg_tx();
void ivc_echo();
void ivc_notify_remote();

#endif