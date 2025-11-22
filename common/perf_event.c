#define _GNU_SOURCE
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

static int perf_fd = -1;

static int perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                           int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

void init_cycle_counter(void) {
    struct perf_event_attr pe = {0};
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    pe.disabled = 0;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    perf_fd = perf_event_open(&pe, 0, -1, -1, 0);
}

uint64_t get_cycle(void) {
    if (perf_fd == -1)
        init_cycle_counter();

    uint64_t count = 0;
    if (read(perf_fd, &count, sizeof(count)) != sizeof(count))
        return 0;
    return count;
}

uint64_t hal_get_time(void) {
    return get_cycle();
}