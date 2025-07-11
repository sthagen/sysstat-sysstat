/*
 * activity.c: Define system activities available for sar/sadc.
 * (C) 1999-2025 by Sebastien GODARD (sysstat <at> orange.fr)
 *
 ***************************************************************************
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published  by  the *
 * Free Software Foundation; either version 2 of the License, or (at  your *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it  will  be  useful,  but *
 * WITHOUT ANY WARRANTY; without the implied warranty  of  MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA              *
 ***************************************************************************
 */

#include "sa.h"

#ifdef SOURCE_SAR
#include "pr_stats.h"
#endif

#ifdef SOURCE_SADF
#include "rndr_stats.h"
#include "xml_stats.h"
#include "json_stats.h"
#include "svg_stats.h"
#include "raw_stats.h"
#include "pcp_stats.h"
#endif

/*
 ***************************************************************************
 * Definitions of system activities.
 * See sa.h file for activity structure definition.
 * Activity structure doesn't matter for daily data files.
 ***************************************************************************
 */

/*
 * Bitmaps needed by activities.
 * Remember to allocate them before use!
 */

/* CPU bitmap */
struct act_bitmap cpu_bitmap = {
	.b_array	= NULL,
	.b_size		= NR_CPUS
};


/*
 * CPU statistics. Switch: -u
 * This is the only activity which *must* be collected by sadc
 * so that uptime can be filled.
 */
struct activity cpu_act = {
	.id		= A_CPU,
	.options	= AO_COLLECTED + AO_COUNTED + AO_PERSISTENT +
			  AO_MULTIPLE_OUTPUTS + AO_GRAPH_PER_ITEM +
			  AO_ALWAYS_COUNTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 0,	/* wrap_get_cpu_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_stat_cpu,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_cpu_stats,
	.f_print_avg	= print_cpu_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "CPU;%user;%nice;%system;%iowait;%steal;%idle|"
		          "CPU;%usr;%nice;%sys;%iowait;%steal;%irq;%soft;%guest;%gnice;%idle",
#endif
	.gtypes_nr	= {STATS_CPU_ULL, STATS_CPU_UL, STATS_CPU_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_cpu_stats,
	.f_xml_print	= xml_print_cpu_stats,
	.f_json_print	= json_print_cpu_stats,
	.f_svg_print	= svg_print_cpu_stats,
	.f_raw_print	= raw_print_cpu_stats,
	.f_pcp_print	= pcp_print_cpu_stats,
	.f_count_new	= NULL,
	.desc		= "CPU utilization",
#endif
	.name		= "A_CPU",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= NR_CPUS + 1,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_CPU_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_CPU_SIZE,
	.msize		= STATS_CPU_SIZE,
	.opt_flags	= AO_F_CPU_DEF,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= &cpu_bitmap
};

/* Process (task) creation and context switch activity. Switch: -w */
struct activity pcsw_act = {
	.id		= A_PCSW,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_stat_pcsw,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pcsw_stats,
	.f_print_avg	= print_pcsw_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "proc/s;cswch/s",
#endif
	.gtypes_nr	= {STATS_PCSW_ULL, STATS_PCSW_UL, STATS_PCSW_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pcsw_stats,
	.f_xml_print	= xml_print_pcsw_stats,
	.f_json_print	= json_print_pcsw_stats,
	.f_svg_print	= svg_print_pcsw_stats,
	.f_raw_print	= raw_print_pcsw_stats,
	.f_pcp_print	= pcp_print_pcsw_stats,
	.f_count_new	= NULL,
	.desc		= "Task creation and switching activity",
#endif
	.name		= "A_PCSW",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_PCSW_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PCSW_SIZE,
	.msize		= STATS_PCSW_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Interrupts statistics. Switch: -I */
struct activity irq_act = {
	.id		= A_IRQ,
	.options	= AO_COUNTED + AO_MATRIX + AO_PERSISTENT,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_INT,
#ifdef SOURCE_SADC
	.f_count_index	= 0,	/* wrap_get_cpu_nr() */
	.f_count2_index	= 1,	/* wrap_get_irq_nr() */
	.f_read		= wrap_read_stat_irq,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_irq_stats,
	.f_print_avg	= print_irq_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "INTR;CPU*",
#endif
	.gtypes_nr	= {STATS_IRQ_ULL, STATS_IRQ_UL, STATS_IRQ_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_irq_stats,
	.f_xml_print	= xml_print_irq_stats,
	.f_json_print	= json_print_irq_stats,
	.f_svg_print	= NULL,
	.f_raw_print	= raw_print_irq_stats,
	.f_pcp_print	= pcp_print_irq_stats,
	.f_count_new	= count_new_int,
	.desc		= "Interrupts statistics",
#endif
	.name		= "A_IRQ",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 0,
	.nr_ini		= -1,	/* Nr of CPU */
	.nr2		= -1,	/* Nr of int */
	.nr_max		= NR_CPUS + 1,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_IRQ_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_IRQ_SIZE,
	.msize		= STATS_IRQ_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= &cpu_bitmap
};

/* Swapping activity. Switch: -W */
struct activity swap_act = {
	.id		= A_SWAP,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_swap,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_swap_stats,
	.f_print_avg	= print_swap_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "pswpin/s;pswpout/s",
#endif
	.gtypes_nr	= {STATS_SWAP_ULL, STATS_SWAP_UL, STATS_SWAP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_swap_stats,
	.f_xml_print	= xml_print_swap_stats,
	.f_json_print	= json_print_swap_stats,
	.f_svg_print	= svg_print_swap_stats,
	.f_raw_print	= raw_print_swap_stats,
	.f_pcp_print	= pcp_print_swap_stats,
	.f_count_new	= NULL,
	.desc		= "Swap activity",
#endif
	.name		= "A_SWAP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_SWAP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_SWAP_SIZE,
	.msize		= STATS_SWAP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Paging activity. Switch: -B */
struct activity paging_act = {
	.id		= A_PAGE,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_paging,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_paging_stats,
	.f_print_avg	= print_paging_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "pgpgin/s;pgpgout/s;fault/s;majflt/s;"
			  "pgfree/s;pgscank/s;pgscand/s;pgsteal/s;"
			  "pgprom/s;pgdem/s",
#endif
	.gtypes_nr	= {STATS_PAGING_ULL, STATS_PAGING_UL, STATS_PAGING_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_paging_stats,
	.f_xml_print	= xml_print_paging_stats,
	.f_json_print	= json_print_paging_stats,
	.f_svg_print	= svg_print_paging_stats,
	.f_raw_print	= raw_print_paging_stats,
	.f_pcp_print	= pcp_print_paging_stats,
	.f_count_new	= NULL,
	.desc		= "Paging activity",
#endif
	.name		= "A_PAGE",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_PAGING_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PAGING_SIZE,
	.msize		= STATS_PAGING_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* I/O and transfer rate activity. Switch: -b */
struct activity io_act = {
	.id		= A_IO,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_io,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_io_stats,
	.f_print_avg	= print_io_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "tps;rtps;wtps;dtps;bread/s;bwrtn/s;bdscd/s",
#endif
	.gtypes_nr	= {STATS_IO_ULL, STATS_IO_UL, STATS_IO_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_io_stats,
	.f_xml_print	= xml_print_io_stats,
	.f_json_print	= json_print_io_stats,
	.f_svg_print	= svg_print_io_stats,
	.f_raw_print	= raw_print_io_stats,
	.f_pcp_print	= pcp_print_io_stats,
	.f_count_new	= NULL,
	.desc		= "I/O and transfer rate statistics",
#endif
	.name		= "A_IO",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_IO_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_IO_SIZE,
	.msize		= STATS_IO_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Memory and swap space utilization activity. Switch: -r */
struct activity memory_act = {
	.id		= A_MEMORY,
	.options	= AO_COLLECTED + AO_MULTIPLE_OUTPUTS,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_meminfo,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_memory_stats,
	.f_print_avg	= print_avg_memory_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "kbmemfree;kbavail;kbmemused;%memused;kbbuffers;kbcached;kbcommit;"
			  "%commit;kbactive;kbinact;kbdirty;kbshmem&kbanonpg;kbslab;"
			  "kbkstack;kbpgtbl;kbvmused|"
			  "kbswpfree;kbswpused;%swpused;kbswpcad;%swpcad",
#endif
	.gtypes_nr	= {STATS_MEMORY_ULL, STATS_MEMORY_UL, STATS_MEMORY_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_memory_stats,
	.f_xml_print	= xml_print_memory_stats,
	.f_json_print	= json_print_memory_stats,
	.f_svg_print	= svg_print_memory_stats,
	.f_raw_print	= raw_print_memory_stats,
	.f_pcp_print	= pcp_print_memory_stats,
	.f_count_new	= NULL,
	.desc		= "Memory and/or swap utilization",
#endif
	.name		= "A_MEMORY",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 9,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_MEMORY_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_MEMORY_SIZE,
	.msize		= STATS_MEMORY_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Kernel tables activity. Switch: -v */
struct activity ktables_act = {
	.id		= A_KTABLES,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_kernel_tables,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_ktables_stats,
	.f_print_avg	= print_avg_ktables_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "dentunusd;file-nr;inode-nr;pty-nr",
#endif
	.gtypes_nr	= {STATS_KTABLES_ULL, STATS_KTABLES_UL, STATS_KTABLES_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_ktables_stats,
	.f_xml_print	= xml_print_ktables_stats,
	.f_json_print	= json_print_ktables_stats,
	.f_svg_print	= svg_print_ktables_stats,
	.f_raw_print	= raw_print_ktables_stats,
	.f_pcp_print	= pcp_print_ktables_stats,
	.f_count_new	= NULL,
	.desc		= "Kernel tables statistics",
#endif
	.name		= "A_KTABLES",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_KTABLES_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_KTABLES_SIZE,
	.msize		= STATS_KTABLES_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Queue and load activity. Switch: -q LOAD */
struct activity queue_act = {
	.id		= A_QUEUE,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_loadavg,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_queue_stats,
	.f_print_avg	= print_avg_queue_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "runq-sz;plist-sz;ldavg-1;ldavg-5;ldavg-15;blocked",
#endif
	.gtypes_nr	= {STATS_QUEUE_ULL, STATS_QUEUE_UL, STATS_QUEUE_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_queue_stats,
	.f_xml_print	= xml_print_queue_stats,
	.f_json_print	= json_print_queue_stats,
	.f_svg_print	= svg_print_queue_stats,
	.f_raw_print	= raw_print_queue_stats,
	.f_pcp_print	= pcp_print_queue_stats,
	.f_count_new	= NULL,
	.desc		= "Queue length and load average statistics",
#endif
	.name		= "A_QUEUE",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 3,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_QUEUE_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_QUEUE_SIZE,
	.msize		= STATS_QUEUE_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Serial lines activity. Switch: -y */
struct activity serial_act = {
	.id		= A_SERIAL,
	.options	= AO_COLLECTED + AO_COUNTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 2,	/* wrap_get_serial_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_tty_driver_serial,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_serial_stats,
	.f_print_avg	= print_serial_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "TTY;rcvin/s;xmtin/s;framerr/s;prtyerr/s;brk/s;ovrun/s",
#endif
	.gtypes_nr	= {STATS_SERIAL_ULL, STATS_SERIAL_UL, STATS_SERIAL_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_serial_stats,
	.f_xml_print	= xml_print_serial_stats,
	.f_json_print	= json_print_serial_stats,
	.f_svg_print	= NULL,
	.f_raw_print	= raw_print_serial_stats,
	.f_pcp_print	= pcp_print_serial_stats,
	.f_count_new	= NULL,
	.desc		= "TTY devices statistics",
#endif
	.name		= "A_SERIAL",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 0,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_SERIAL_LINES,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_SERIAL_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_SERIAL_SIZE,
	.msize		= STATS_SERIAL_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Block devices activity. Switch: -d */
struct activity disk_act = {
	.id		= A_DISK,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_DISK,
#ifdef SOURCE_SADC
	.f_count_index	= 3,	/* wrap_get_disk_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_disk,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_disk_stats,
	.f_print_avg	= print_disk_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "DEV;tps;rkB/s;wkB/s;dkB/s;areq-sz;aqu-sz;await;%util",
#endif
	.gtypes_nr	= {STATS_DISK_ULL, STATS_DISK_UL, STATS_DISK_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_disk_stats,
	.f_xml_print	= xml_print_disk_stats,
	.f_json_print	= json_print_disk_stats,
	.f_svg_print	= svg_print_disk_stats,
	.f_raw_print	= raw_print_disk_stats,
	.f_pcp_print	= pcp_print_disk_stats,
	.f_count_new	= count_new_disk,
	.desc		= "Block devices statistics",
#endif
	.name		= "A_DISK",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 5,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_DISKS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_DISK_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_DISK_SIZE,
	.msize		= STATS_DISK_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Network interfaces activity. Switch: -n DEV */
struct activity net_dev_act = {
	.id		= A_NET_DEV,
	.options	= AO_COLLECTED + AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE + 3,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 4,	/* wrap_get_iface_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_dev,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_dev_stats,
	.f_print_avg	= print_net_dev_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "IFACE;rxpck/s;txpck/s;rxkB/s;txkB/s;rxcmp/s;txcmp/s;rxmcst/s;%ifutil",
#endif
	.gtypes_nr	= {STATS_NET_DEV_ULL, STATS_NET_DEV_UL, STATS_NET_DEV_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_dev_stats,
	.f_xml_print	= xml_print_net_dev_stats,
	.f_json_print	= json_print_net_dev_stats,
	.f_svg_print	= svg_print_net_dev_stats,
	.f_raw_print	= raw_print_net_dev_stats,
	.f_pcp_print	= pcp_print_net_dev_stats,
	.f_count_new	= count_new_net_dev,
	.desc		= "Network interfaces statistics",
#endif
	.name		= "A_NET_DEV",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_IFACES,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_DEV_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_DEV_SIZE,
	.msize		= STATS_NET_DEV_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Network interfaces (errors) activity. Switch: -n EDEV */
struct activity net_edev_act = {
	.id		= A_NET_EDEV,
	.options	= AO_COLLECTED + AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 4,	/* wrap_get_iface_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_edev,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_edev_stats,
	.f_print_avg	= print_net_edev_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "IFACE;rxerr/s;txerr/s;coll/s;rxdrop/s;txdrop/s;"
		          "txcarr/s;rxfram/s;rxfifo/s;txfifo/s",
#endif
	.gtypes_nr	= {STATS_NET_EDEV_ULL, STATS_NET_EDEV_UL, STATS_NET_EDEV_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_edev_stats,
	.f_xml_print	= xml_print_net_edev_stats,
	.f_json_print	= json_print_net_edev_stats,
	.f_svg_print	= svg_print_net_edev_stats,
	.f_raw_print	= raw_print_net_edev_stats,
	.f_pcp_print	= pcp_print_net_edev_stats,
	.f_count_new	= count_new_net_edev,
	.desc		= "Network interfaces errors statistics",
#endif
	.name		= "A_NET_EDEV",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_IFACES,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_EDEV_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_EDEV_SIZE,
	.msize		= STATS_NET_EDEV_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* NFS client activity. Switch: -n NFS */
struct activity net_nfs_act = {
	.id		= A_NET_NFS,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_nfs,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_nfs_stats,
	.f_print_avg	= print_net_nfs_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "call/s;retrans/s;read/s;write/s;access/s;getatt/s",
#endif
	.gtypes_nr	= {STATS_NET_NFS_ULL, STATS_NET_NFS_UL, STATS_NET_NFS_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_nfs_stats,
	.f_xml_print	= xml_print_net_nfs_stats,
	.f_json_print	= json_print_net_nfs_stats,
	.f_svg_print	= svg_print_net_nfs_stats,
	.f_raw_print	= raw_print_net_nfs_stats,
	.f_pcp_print	= pcp_print_net_nfs_stats,
	.f_count_new	= NULL,
	.desc		= "NFS client statistics",
#endif
	.name		= "A_NET_NFS",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 3,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_NFS_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_NFS_SIZE,
	.msize		= STATS_NET_NFS_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* NFS server activity. Switch: -n NFSD */
struct activity net_nfsd_act = {
	.id		= A_NET_NFSD,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_nfsd,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_nfsd_stats,
	.f_print_avg	= print_net_nfsd_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "scall/s;badcall/s;packet/s;udp/s;tcp/s;hit/s;miss/s;"
		          "sread/s;swrite/s;saccess/s;sgetatt/s",
#endif
	.gtypes_nr	= {STATS_NET_NFSD_ULL, STATS_NET_NFSD_UL, STATS_NET_NFSD_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_nfsd_stats,
	.f_xml_print	= xml_print_net_nfsd_stats,
	.f_json_print	= json_print_net_nfsd_stats,
	.f_svg_print	= svg_print_net_nfsd_stats,
	.f_raw_print	= raw_print_net_nfsd_stats,
	.f_pcp_print	= pcp_print_net_nfsd_stats,
	.f_count_new	= NULL,
	.desc		= "NFS server statistics",
#endif
	.name		= "A_NET_NFSD",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 5,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_NFSD_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_NFSD_SIZE,
	.msize		= STATS_NET_NFSD_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Network sockets activity. Switch: -n SOCK */
struct activity net_sock_act = {
	.id		= A_NET_SOCK,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_sock,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_sock_stats,
	.f_print_avg	= print_avg_net_sock_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "totsck;tcpsck;udpsck;rawsck;ip-frag;tcp-tw",
#endif
	.gtypes_nr	= {STATS_NET_SOCK_ULL, STATS_NET_SOCK_UL, STATS_NET_SOCK_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_sock_stats,
	.f_xml_print	= xml_print_net_sock_stats,
	.f_json_print	= json_print_net_sock_stats,
	.f_svg_print	= svg_print_net_sock_stats,
	.f_raw_print	= raw_print_net_sock_stats,
	.f_pcp_print	= pcp_print_net_sock_stats,
	.f_count_new	= NULL,
	.desc		= "IPv4 sockets statistics",
#endif
	.name		= "A_NET_SOCK",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_SOCK_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_SOCK_SIZE,
	.msize		= STATS_NET_SOCK_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* IP network traffic activity. Switch: -n IP */
struct activity net_ip_act = {
	.id		= A_NET_IP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_ip,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_ip_stats,
	.f_print_avg	= print_net_ip_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "irec/s;fwddgm/s;idel/s;orq/s;asmrq/s;asmok/s;fragok/s;fragcrt/s",
#endif
	.gtypes_nr	= {STATS_NET_IP_ULL, STATS_NET_IP_UL, STATS_NET_IP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_ip_stats,
	.f_xml_print	= xml_print_net_ip_stats,
	.f_json_print	= json_print_net_ip_stats,
	.f_svg_print	= svg_print_net_ip_stats,
	.f_raw_print	= raw_print_net_ip_stats,
	.f_pcp_print	= pcp_print_net_ip_stats,
	.f_count_new	= NULL,
	.desc		= "IPv4 traffic statistics",
#endif
	.name		= "A_NET_IP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 3,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_IP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_IP_SIZE,
	.msize		= STATS_NET_IP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* IP network traffic (errors) activity. Switch: -n EIP */
struct activity net_eip_act = {
	.id		= A_NET_EIP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_eip,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_eip_stats,
	.f_print_avg	= print_net_eip_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "ihdrerr/s;iadrerr/s;iukwnpr/s;idisc/s;odisc/s;onort/s;asmf/s;fragf/s",
#endif
	.gtypes_nr	= {STATS_NET_EIP_ULL, STATS_NET_EIP_UL, STATS_NET_EIP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_eip_stats,
	.f_xml_print	= xml_print_net_eip_stats,
	.f_json_print	= json_print_net_eip_stats,
	.f_svg_print	= svg_print_net_eip_stats,
	.f_raw_print	= raw_print_net_eip_stats,
	.f_pcp_print	= pcp_print_net_eip_stats,
	.f_count_new	= NULL,
	.desc		= "IPv4 traffic errors statistics",
#endif
	.name		= "A_NET_EIP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 3,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_EIP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_EIP_SIZE,
	.msize		= STATS_NET_EIP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* ICMP network traffic activity. Switch: -n ICMP */
struct activity net_icmp_act = {
	.id		= A_NET_ICMP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_icmp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_icmp_stats,
	.f_print_avg	= print_net_icmp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "imsg/s;omsg/s;iech/s;iechr/s;oech/s;oechr/s;itm/s;itmr/s;otm/s;"
		          "otmr/s;iadrmk/s;iadrmkr/s;oadrmk/s;oadrmkr/s",
#endif
	.gtypes_nr	= {STATS_NET_ICMP_ULL, STATS_NET_ICMP_UL, STATS_NET_ICMP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_icmp_stats,
	.f_xml_print	= xml_print_net_icmp_stats,
	.f_json_print	= json_print_net_icmp_stats,
	.f_svg_print	= svg_print_net_icmp_stats,
	.f_raw_print	= raw_print_net_icmp_stats,
	.f_pcp_print	= pcp_print_net_icmp_stats,
	.f_count_new	= NULL,
	.desc		= "ICMPv4 traffic statistics",
#endif
	.name		= "A_NET_ICMP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_ICMP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_ICMP_SIZE,
	.msize		= STATS_NET_ICMP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* ICMP network traffic (errors) activity. Switch: -n EICMP */
struct activity net_eicmp_act = {
	.id		= A_NET_EICMP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_eicmp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_eicmp_stats,
	.f_print_avg	= print_net_eicmp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "ierr/s;oerr/s;idstunr/s;odstunr/s;itmex/s;otmex/s;"
		          "iparmpb/s;oparmpb/s;isrcq/s;osrcq/s;iredir/s;oredir/s",
#endif
	.gtypes_nr	= {STATS_NET_EICMP_ULL, STATS_NET_EICMP_UL, STATS_NET_EICMP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_eicmp_stats,
	.f_xml_print	= xml_print_net_eicmp_stats,
	.f_json_print	= json_print_net_eicmp_stats,
	.f_svg_print	= svg_print_net_eicmp_stats,
	.f_raw_print	= raw_print_net_eicmp_stats,
	.f_pcp_print	= pcp_print_net_eicmp_stats,
	.f_count_new	= NULL,
	.desc		= "ICMPv4 traffic errors statistics",
#endif
	.name		= "A_NET_EICMP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 6,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_EICMP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_EICMP_SIZE,
	.msize		= STATS_NET_EICMP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* TCP network traffic activity. Switch: -n TCP */
struct activity net_tcp_act = {
	.id		= A_NET_TCP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_tcp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_tcp_stats,
	.f_print_avg	= print_net_tcp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "active/s;passive/s;iseg/s;oseg/s",
#endif
	.gtypes_nr	= {STATS_NET_TCP_ULL, STATS_NET_TCP_UL, STATS_NET_TCP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_tcp_stats,
	.f_xml_print	= xml_print_net_tcp_stats,
	.f_json_print	= json_print_net_tcp_stats,
	.f_svg_print	= svg_print_net_tcp_stats,
	.f_raw_print	= raw_print_net_tcp_stats,
	.f_pcp_print	= pcp_print_net_tcp_stats,
	.f_count_new	= NULL,
	.desc		= "TCPv4 traffic statistics",
#endif
	.name		= "A_NET_TCP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_TCP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_TCP_SIZE,
	.msize		= STATS_NET_TCP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* TCP network traffic (errors) activity. Switch: -n ETCP */
struct activity net_etcp_act = {
	.id		= A_NET_ETCP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_etcp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_etcp_stats,
	.f_print_avg	= print_net_etcp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "atmptf/s;estres/s;retrseg/s;isegerr/s;orsts/s",
#endif
	.gtypes_nr	= {STATS_NET_ETCP_ULL, STATS_NET_ETCP_UL, STATS_NET_ETCP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_etcp_stats,
	.f_xml_print	= xml_print_net_etcp_stats,
	.f_json_print	= json_print_net_etcp_stats,
	.f_svg_print	= svg_print_net_etcp_stats,
	.f_raw_print	= raw_print_net_etcp_stats,
	.f_pcp_print	= pcp_print_net_etcp_stats,
	.f_count_new	= NULL,
	.desc		= "TCPv4 traffic errors statistics",
#endif
	.name		= "A_NET_ETCP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_ETCP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_ETCP_SIZE,
	.msize		= STATS_NET_ETCP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* UDP network traffic activity. Switch: -n UDP */
struct activity net_udp_act = {
	.id		= A_NET_UDP,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_SNMP,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_udp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_udp_stats,
	.f_print_avg	= print_net_udp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "idgm/s;odgm/s;noport/s;idgmerr/s",
#endif
	.gtypes_nr	= {STATS_NET_UDP_ULL, STATS_NET_UDP_UL, STATS_NET_UDP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_udp_stats,
	.f_xml_print	= xml_print_net_udp_stats,
	.f_json_print	= json_print_net_udp_stats,
	.f_svg_print	= svg_print_net_udp_stats,
	.f_raw_print	= raw_print_net_udp_stats,
	.f_pcp_print	= pcp_print_net_udp_stats,
	.f_count_new	= NULL,
	.desc		= "UDPv4 traffic statistics",
#endif
	.name		= "A_NET_UDP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_UDP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_UDP_SIZE,
	.msize		= STATS_NET_UDP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* IPv6 sockets activity. Switch: -n SOCK6 */
struct activity net_sock6_act = {
	.id		= A_NET_SOCK6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_sock6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_sock6_stats,
	.f_print_avg	= print_avg_net_sock6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "tcp6sck;udp6sck;raw6sck;ip6-frag",
#endif
	.gtypes_nr	= {STATS_NET_SOCK6_ULL, STATS_NET_SOCK6_UL, STATS_NET_SOCK6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_sock6_stats,
	.f_xml_print	= xml_print_net_sock6_stats,
	.f_json_print	= json_print_net_sock6_stats,
	.f_svg_print	= svg_print_net_sock6_stats,
	.f_raw_print	= raw_print_net_sock6_stats,
	.f_pcp_print	= pcp_print_net_sock6_stats,
	.f_count_new	= NULL,
	.desc		= "IPv6 sockets statistics",
#endif
	.name		= "A_NET_SOCK6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_SOCK6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_SOCK6_SIZE,
	.msize		= STATS_NET_SOCK6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* IPv6 network traffic activity. Switch: -n IP6 */
struct activity net_ip6_act = {
	.id		= A_NET_IP6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_ip6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_ip6_stats,
	.f_print_avg	= print_net_ip6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "irec6/s;fwddgm6/s;idel6/s;orq6/s;asmrq6/s;asmok6/s;"
			  "imcpck6/s;omcpck6/s;fragok6/s;fragcr6/s",
#endif
	.gtypes_nr	= {STATS_NET_IP6_ULL, STATS_NET_IP6_UL, STATS_NET_IP6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_ip6_stats,
	.f_xml_print	= xml_print_net_ip6_stats,
	.f_json_print	= json_print_net_ip6_stats,
	.f_svg_print	= svg_print_net_ip6_stats,
	.f_raw_print	= raw_print_net_ip6_stats,
	.f_pcp_print	= pcp_print_net_ip6_stats,
	.f_count_new	= NULL,
	.desc		= "IPv6 traffic statistics",
#endif
	.name		= "A_NET_IP6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_IP6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_IP6_SIZE,
	.msize		= STATS_NET_IP6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* IPv6 network traffic (errors) activity. Switch: -n EIP6 */
struct activity net_eip6_act = {
	.id		= A_NET_EIP6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE + 2,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_eip6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_eip6_stats,
	.f_print_avg	= print_net_eip6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "ihdrer6/s;iadrer6/s;iukwnp6/s;i2big6/s;idisc6/s;odisc6/s;"
			  "inort6/s;onort6/s;asmf6/s;fragf6/s;itrpck6/s",
#endif
	.gtypes_nr	= {STATS_NET_EIP6_ULL, STATS_NET_EIP6_UL, STATS_NET_EIP6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_eip6_stats,
	.f_xml_print	= xml_print_net_eip6_stats,
	.f_json_print	= json_print_net_eip6_stats,
	.f_svg_print	= svg_print_net_eip6_stats,
	.f_raw_print	= raw_print_net_eip6_stats,
	.f_pcp_print	= pcp_print_net_eip6_stats,
	.f_count_new	= NULL,
	.desc		= "IPv6 traffic errors statistics",
#endif
	.name		= "A_NET_EIP6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_EIP6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_EIP6_SIZE,
	.msize		= STATS_NET_EIP6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* ICMPv6 network traffic activity. Switch: -n ICMP6 */
struct activity net_icmp6_act = {
	.id		= A_NET_ICMP6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_icmp6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_icmp6_stats,
	.f_print_avg	= print_net_icmp6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "imsg6/s;omsg6/s;iech6/s;iechr6/s;oechr6/s;igmbq6/s;igmbr6/s;ogmbr6/s;"
			  "igmbrd6/s;ogmbrd6/s;irtsol6/s;ortsol6/s;irtad6/s;inbsol6/s;onbsol6/s;"
			  "inbad6/s;onbad6/s",
#endif
	.gtypes_nr	= {STATS_NET_ICMP6_ULL, STATS_NET_ICMP6_UL, STATS_NET_ICMP6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_icmp6_stats,
	.f_xml_print	= xml_print_net_icmp6_stats,
	.f_json_print	= json_print_net_icmp6_stats,
	.f_svg_print	= svg_print_net_icmp6_stats,
	.f_raw_print	= raw_print_net_icmp6_stats,
	.f_pcp_print	= pcp_print_net_icmp6_stats,
	.f_count_new	= NULL,
	.desc		= "ICMPv6 traffic statistics",
#endif
	.name		= "A_NET_ICMP6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 5,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_ICMP6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_ICMP6_SIZE,
	.msize		= STATS_NET_ICMP6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* ICMPv6 network traffic (errors) activity. Switch: -n EICMP6 */
struct activity net_eicmp6_act = {
	.id		= A_NET_EICMP6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_eicmp6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_eicmp6_stats,
	.f_print_avg	= print_net_eicmp6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "ierr6/s;idtunr6/s;odtunr6/s;itmex6/s;otmex6/s;"
		          "iprmpb6/s;oprmpb6/s;iredir6/s;oredir6/s;ipck2b6/s;opck2b6/s",
#endif
	.gtypes_nr	= {STATS_NET_EICMP6_ULL, STATS_NET_EICMP6_UL, STATS_NET_EICMP6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_eicmp6_stats,
	.f_xml_print	= xml_print_net_eicmp6_stats,
	.f_json_print	= json_print_net_eicmp6_stats,
	.f_svg_print	= svg_print_net_eicmp6_stats,
	.f_raw_print	= raw_print_net_eicmp6_stats,
	.f_pcp_print	= pcp_print_net_eicmp6_stats,
	.f_count_new	= NULL,
	.desc		= "ICMPv6 traffic errors statistics",
#endif
	.name		= "A_NET_EICMP6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 6,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_EICMP6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_EICMP6_SIZE,
	.msize		= STATS_NET_EICMP6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* UDPv6 network traffic activity. Switch: -n UDP6 */
struct activity net_udp6_act = {
	.id		= A_NET_UDP6,
	.options	= AO_NULL,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_IPV6,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_net_udp6,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_net_udp6_stats,
	.f_print_avg	= print_net_udp6_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "idgm6/s;odgm6/s;noport6/s;idgmer6/s",
#endif
	.gtypes_nr	= {STATS_NET_UDP6_ULL, STATS_NET_UDP6_UL, STATS_NET_UDP6_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_net_udp6_stats,
	.f_xml_print	= xml_print_net_udp6_stats,
	.f_json_print	= json_print_net_udp6_stats,
	.f_svg_print	= svg_print_net_udp6_stats,
	.f_raw_print	= raw_print_net_udp6_stats,
	.f_pcp_print	= pcp_print_net_udp6_stats,
	.f_count_new	= NULL,
	.desc		= "UDPv6 traffic statistics",
#endif
	.name		= "A_NET_UDP6",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_NET_UDP6_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_NET_UDP6_SIZE,
	.msize		= STATS_NET_UDP6_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* CPU frequency. Switch: -m CPU */
struct activity pwr_cpufreq_act = {
	.id		= A_PWR_CPU,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 0,	/* wrap_get_cpu_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_cpuinfo,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_cpufreq_stats,
	.f_print_avg	= print_avg_pwr_cpufreq_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "CPU;MHz",
#endif
	.gtypes_nr	= {STATS_PWR_CPUFREQ_ULL, STATS_PWR_CPUFREQ_UL, STATS_PWR_CPUFREQ_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_cpufreq_stats,
	.f_xml_print	= xml_print_pwr_cpufreq_stats,
	.f_json_print	= json_print_pwr_cpufreq_stats,
	.f_svg_print	= svg_print_pwr_cpufreq_stats,
	.f_raw_print	= raw_print_pwr_cpufreq_stats,
	.f_pcp_print	= pcp_print_pwr_cpufreq_stats,
	.f_count_new	= NULL,
	.desc		= "CPU clock frequency",
#endif
	.name		= "A_PWR_CPU",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= NR_CPUS + 1,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_CPUFREQ_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_CPUFREQ_SIZE,
	.msize		= STATS_PWR_CPUFREQ_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= &cpu_bitmap
};

/* Fan. Switch: -m FAN */
struct activity pwr_fan_act = {
	.id		= A_PWR_FAN,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 5,	/* wrap_get_fan_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_fan,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_fan_stats,
	.f_print_avg	= print_avg_pwr_fan_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "FAN;DEVICE;rpm;drpm",
#endif
	.gtypes_nr	= {STATS_PWR_FAN_ULL, STATS_PWR_FAN_UL, STATS_PWR_FAN_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_fan_stats,
	.f_xml_print	= xml_print_pwr_fan_stats,
	.f_json_print	= json_print_pwr_fan_stats,
	.f_svg_print	= svg_print_pwr_fan_stats,
	.f_raw_print	= raw_print_pwr_fan_stats,
	.f_pcp_print	= pcp_print_pwr_fan_stats,
	.f_count_new	= NULL,
	.desc		= "Fans speed",
#endif
	.name		= "A_PWR_FAN",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_FANS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_FAN_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_FAN_SIZE,
	.msize		= STATS_PWR_FAN_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Temperature. Switch: -m TEMP */
struct activity pwr_temp_act = {
	.id		= A_PWR_TEMP,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 6,	/* wrap_get_temp_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_temp,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_temp_stats,
	.f_print_avg	= print_avg_pwr_temp_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "TEMP;DEVICE;degC;%temp",
#endif
	.gtypes_nr	= {STATS_PWR_TEMP_ULL, STATS_PWR_TEMP_UL, STATS_PWR_TEMP_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_temp_stats,
	.f_xml_print	= xml_print_pwr_temp_stats,
	.f_json_print	= json_print_pwr_temp_stats,
	.f_svg_print	= svg_print_pwr_temp_stats,
	.f_raw_print	= raw_print_pwr_temp_stats,
	.f_pcp_print	= pcp_print_pwr_temp_stats,
	.f_count_new	= NULL,
	.desc		= "Devices temperature",
#endif
	.name		= "A_PWR_TEMP",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_TEMP_SENSORS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_TEMP_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_TEMP_SIZE,
	.msize		= STATS_PWR_TEMP_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Voltage inputs. Switch: -m IN */
struct activity pwr_in_act = {
	.id		= A_PWR_IN,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 7,	/* wrap_get_in_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_in,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_in_stats,
	.f_print_avg	= print_avg_pwr_in_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "IN;DEVICE;inV;%in",
#endif
	.gtypes_nr	= {STATS_PWR_IN_ULL, STATS_PWR_IN_UL, STATS_PWR_IN_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_in_stats,
	.f_xml_print	= xml_print_pwr_in_stats,
	.f_json_print	= json_print_pwr_in_stats,
	.f_svg_print	= svg_print_pwr_in_stats,
	.f_raw_print	= raw_print_pwr_in_stats,
	.f_pcp_print	= pcp_print_pwr_in_stats,
	.f_count_new	= NULL,
	.desc		= "Voltage inputs statistics",
#endif
	.name		= "A_PWR_IN",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_IN_SENSORS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_IN_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_IN_SIZE,
	.msize		= STATS_PWR_IN_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Hugepages activity. Switch: -H */
struct activity huge_act = {
	.id		= A_HUGE,
	.options	= AO_COLLECTED,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= -1,
	.f_count2_index	= -1,
	.f_read		= wrap_read_meminfo_huge,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_huge_stats,
	.f_print_avg	= print_avg_huge_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "kbhugfree;kbhugused;%hugused;kbhugrsvd;kbhugsurp",
#endif
	.gtypes_nr	= {STATS_HUGE_ULL, STATS_HUGE_UL, STATS_HUGE_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_huge_stats,
	.f_xml_print	= xml_print_huge_stats,
	.f_json_print	= json_print_huge_stats,
	.f_svg_print	= svg_print_huge_stats,
	.f_raw_print	= raw_print_huge_stats,
	.f_pcp_print	= pcp_print_huge_stats,
	.f_count_new	= NULL,
	.desc		= "Huge pages utilization",
#endif
	.name		= "A_HUGE",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_HUGE_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_HUGE_SIZE,
	.msize		= STATS_HUGE_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* CPU weighted frequency. Switch: -m FREQ */
struct activity pwr_wghfreq_act = {
	.id		= A_PWR_FREQ,
	.options	= AO_COUNTED + AO_MATRIX,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 0,	/* wrap_get_cpu_nr() */
	.f_count2_index	= 12,	/* wrap_get_freq_nr() */
	.f_read		= wrap_read_cpu_wghfreq,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_wghfreq_stats,
	.f_print_avg	= print_pwr_wghfreq_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "CPU;wghMHz",
#endif
	.gtypes_nr	= {STATS_PWR_WGHFREQ_ULL, STATS_PWR_WGHFREQ_UL, STATS_PWR_WGHFREQ_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_wghfreq_stats,
	.f_xml_print	= xml_print_pwr_wghfreq_stats,
	.f_json_print	= json_print_pwr_wghfreq_stats,
	.f_svg_print	= NULL,
	.f_raw_print	= raw_print_pwr_wghfreq_stats,
	.f_count_new	= NULL,
	.desc		= "CPU weighted frequency",
#endif
	.name		= "A_PWR_FREQ",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 0,
	.nr_ini		= -1,	/* Nr of CPU */
	.nr2		= -1,	/* Nr of frequencies */
	.nr_max		= NR_CPUS + 1,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_WGHFREQ_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_WGHFREQ_SIZE,
	.msize		= STATS_PWR_WGHFREQ_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= &cpu_bitmap
};

/* USB devices plugged into the system. Switch: -m USB */
struct activity pwr_usb_act = {
	.id		= A_PWR_USB,
	.options	= AO_COUNTED + AO_CLOSE_MARKUP,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 8,	/* wrap_get_usb_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_bus_usb_dev,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_usb_stats,
	.f_print_avg	= print_avg_pwr_usb_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "manufact;product;BUS;idvendor;idprod;maxpower",
#endif
	.gtypes_nr	= {STATS_PWR_USB_ULL, STATS_PWR_USB_UL, STATS_PWR_USB_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_usb_stats,
	.f_xml_print	= xml_print_pwr_usb_stats,
	.f_json_print	= json_print_pwr_usb_stats,
	.f_svg_print	= NULL,
	.f_raw_print	= raw_print_pwr_usb_stats,
	.f_pcp_print	= pcp_print_pwr_usb_stats,
	.f_count_new	= NULL,
	.desc		= "USB devices",
#endif
	.name		= "A_PWR_USB",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 0,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_USB,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_USB_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_USB_SIZE,
	.msize		= STATS_PWR_USB_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Filesystem usage activity. Switch: -F */
struct activity filesystem_act = {
	.id		= A_FS,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM + AO_MULTIPLE_OUTPUTS,
	.magic		= ACTIVITY_MAGIC_BASE + 1,
	.group		= G_XDISK,
#ifdef SOURCE_SADC
	.f_count_index	= 9,	/* wrap_get_filesystem_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_filesystem,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_filesystem_stats,
	.f_print_avg	= print_avg_filesystem_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "FILESYSTEM;MBfsfree;MBfsused;%fsused;%ufsused;Ifree;Iused;%Iused|"
			  "MOUNTPOINT;MBfsfree;MBfsused;%fsused;%ufsused;Ifree;Iused;%Iused",
#endif
	.gtypes_nr	= {STATS_FILESYSTEM_ULL, STATS_FILESYSTEM_UL, STATS_FILESYSTEM_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_filesystem_stats,
	.f_xml_print	= xml_print_filesystem_stats,
	.f_json_print	= json_print_filesystem_stats,
	.f_svg_print	= svg_print_filesystem_stats,
	.f_raw_print	= raw_print_filesystem_stats,
	.f_pcp_print	= pcp_print_filesystem_stats,
	.f_count_new	= count_new_filesystem,
	.desc		= "Filesystems statistics",
#endif
	.name		= "A_FS",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_FS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_FILESYSTEM_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_FILESYSTEM_SIZE,
	.msize		= STATS_FILESYSTEM_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Fibre Channel HBA usage activity. Switch: -n FC */
struct activity fchost_act = {
	.id		= A_NET_FC,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DISK,
#ifdef SOURCE_SADC
	.f_count_index	= 10,	/* wrap_get_fchost_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_fchost,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_fchost_stats,
	.f_print_avg	= print_fchost_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "FCHOST;fch_rxf/s;fch_txf/s;fch_rxw/s;fch_txw/s",
#endif
	.gtypes_nr	= {STATS_FCHOST_ULL, STATS_FCHOST_UL, STATS_FCHOST_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_fchost_stats,
	.f_xml_print	= xml_print_fchost_stats,
	.f_json_print	= json_print_fchost_stats,
	.f_svg_print	= svg_print_fchost_stats,
	.f_raw_print	= raw_print_fchost_stats,
	.f_pcp_print	= pcp_print_fchost_stats,
	.f_count_new	= count_new_fchost,
	.desc		= "Fibre Channel HBA statistics",
#endif
	.name		= "A_NET_FC",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_FCHOSTS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_FCHOST_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_FCHOST_SIZE,
	.msize		= STATS_FCHOST_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Softnet activity. Switch: -n SOFT */
struct activity softnet_act = {
	.id		= A_NET_SOFT,
	.options	= AO_COLLECTED + AO_COUNTED + AO_CLOSE_MARKUP +
			  AO_GRAPH_PER_ITEM + AO_PERSISTENT,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 0,	/* wrap_get_cpu_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_softnet,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_softnet_stats,
	.f_print_avg	= print_avg_softnet_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "CPU;total/s;dropd/s;squeezd/s;rx_rps/s;flw_lim/s;blg_len",
#endif
	.gtypes_nr	= {STATS_SOFTNET_ULL, STATS_SOFTNET_UL, STATS_SOFTNET_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_softnet_stats,
	.f_xml_print	= xml_print_softnet_stats,
	.f_json_print	= json_print_softnet_stats,
	.f_svg_print	= svg_print_softnet_stats,
	.f_raw_print	= raw_print_softnet_stats,
	.f_pcp_print	= pcp_print_softnet_stats,
	.f_count_new	= NULL,
	.desc		= "Software-based network processing statistics",
#endif
	.name		= "A_NET_SOFT",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 3,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= NR_CPUS + 1,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_SOFTNET_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_SOFTNET_SIZE,
	.msize		= STATS_SOFTNET_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= &cpu_bitmap
};

/* Pressure-stall CPU activity. Switch: -q CPU */
struct activity psi_cpu_act = {
	.id		= A_PSI_CPU,
	.options	= AO_COLLECTED + AO_DETECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 11,	/* wrap_detect_psi() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_psicpu,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_psicpu_stats,
	.f_print_avg	= print_avg_psicpu_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "%scpu-10;%scpu-60;%scpu-300;%scpu",
#endif
	.gtypes_nr	= {STATS_PSI_CPU_ULL, STATS_PSI_CPU_UL, STATS_PSI_CPU_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_psicpu_stats,
	.f_xml_print	= xml_print_psicpu_stats,
	.f_json_print	= json_print_psicpu_stats,
	.f_svg_print	= svg_print_psicpu_stats,
	.f_raw_print	= raw_print_psicpu_stats,
	.f_pcp_print	= pcp_print_psicpu_stats,
	.f_count_new	= NULL,
	.desc		= "Pressure-stall CPU statistics",
#endif
	.name		= "A_PSI_CPU",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 2,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_PSI_CPU_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PSI_CPU_SIZE,
	.msize		= STATS_PSI_CPU_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Pressure-stall I/O activity. Switch: -q IO */
struct activity psi_io_act = {
	.id		= A_PSI_IO,
	.options	= AO_COLLECTED + AO_DETECTED,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 11,	/* wrap_detect_psi() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_psiio,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_psiio_stats,
	.f_print_avg	= print_avg_psiio_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "%sio-10;%sio-60;%sio-300;%sio;%fio-10;%fio-60;%fio-300;%fio",
#endif
	.gtypes_nr	= {STATS_PSI_IO_ULL, STATS_PSI_IO_UL, STATS_PSI_IO_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_psiio_stats,
	.f_xml_print	= xml_print_psiio_stats,
	.f_json_print	= json_print_psiio_stats,
	.f_svg_print	= svg_print_psiio_stats,
	.f_raw_print	= raw_print_psiio_stats,
	.f_pcp_print	= pcp_print_psiio_stats,
	.f_count_new	= NULL,
	.desc		= "Pressure-stall I/O statistics",
#endif
	.name		= "A_PSI_IO",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_PSI_IO_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PSI_IO_SIZE,
	.msize		= STATS_PSI_IO_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Pressure-stall memory activity. Switch: -q MEM */
struct activity psi_mem_act = {
	.id		= A_PSI_MEM,
	.options	= AO_COLLECTED + AO_DETECTED + AO_CLOSE_MARKUP,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_DEFAULT,
#ifdef SOURCE_SADC
	.f_count_index	= 11,	/* wrap_detect_psi() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_psimem,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_psimem_stats,
	.f_print_avg	= print_avg_psimem_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "%smem-10;%smem-60;%smem-300;%smem;%fmem-10;%fmem-60;%fmem-300;%fmem",
#endif
	.gtypes_nr	= {STATS_PSI_MEM_ULL, STATS_PSI_MEM_UL, STATS_PSI_MEM_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_psimem_stats,
	.f_xml_print	= xml_print_psimem_stats,
	.f_json_print	= json_print_psimem_stats,
	.f_svg_print	= svg_print_psimem_stats,
	.f_raw_print	= raw_print_psimem_stats,
	.f_pcp_print	= pcp_print_psimem_stats,
	.f_count_new	= NULL,
	.desc		= "Pressure-stall memory statistics",
#endif
	.name		= "A_PSI_MEM",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 4,
	.nr_ini		= 1,
	.nr2		= 1,
	.nr_max		= 1,
	.nr		= {1, 1, 1},
	.nr_allocated	= 0,
	.xnr		= STATS_PSI_MEM_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PSI_MEM_SIZE,
	.msize		= STATS_PSI_MEM_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

/* Battery capacity. Switch: -m BAT */
struct activity pwr_bat_act = {
	.id		= A_PWR_BAT,
	.options	= AO_COUNTED + AO_GRAPH_PER_ITEM,
	.magic		= ACTIVITY_MAGIC_BASE,
	.group		= G_POWER,
#ifdef SOURCE_SADC
	.f_count_index	= 13,	/* wrap_get_bat_nr() */
	.f_count2_index	= -1,
	.f_read		= wrap_read_bat,
#endif
#ifdef SOURCE_SAR
	.f_print	= print_pwr_bat_stats,
	.f_print_avg	= print_avg_pwr_bat_stats,
#endif
#if defined(SOURCE_SAR) || defined(SOURCE_SADF)
	.hdr_line	= "BAT;%cap;cap/min;status",
#endif
	.gtypes_nr	= {STATS_PWR_BAT_ULL, STATS_PWR_BAT_UL, STATS_PWR_BAT_U},
	.ftypes_nr	= {0, 0, 0},
#ifdef SOURCE_SADF
	.f_render	= render_pwr_bat_stats,
	.f_xml_print	= xml_print_pwr_bat_stats,
	.f_json_print	= json_print_pwr_bat_stats,
	.f_svg_print	= svg_print_pwr_bat_stats,
	.f_raw_print	= raw_print_pwr_bat_stats,
	.f_pcp_print	= pcp_print_pwr_bat_stats,
	.f_count_new	= count_new_bat,
	.desc		= "Batteries capacity",
#endif
	.name		= "A_PWR_BAT",
	.item_list	= NULL,
	.item_list_sz	= 0,
	.g_nr		= 1,
	.nr_ini		= -1,
	.nr2		= 1,
	.nr_max		= MAX_NR_BATS,
	.nr		= {-1, -1, -1},
	.nr_allocated	= 0,
	.xnr		= STATS_PWR_BAT_XNR,
	.xdev_list	= NULL,
	.fsize		= STATS_PWR_BAT_SIZE,
	.msize		= STATS_PWR_BAT_SIZE,
	.opt_flags	= 0,
	.buf		= {NULL, NULL, NULL},
	.spmin		= NULL,
	.spmax		= NULL,
	.nr_spalloc	= 0,
	.bitmap		= NULL
};

#ifdef SOURCE_SADC
/*
 * Array of functions used to count number of items.
 */
__nr_t (*f_count[NR_F_COUNT]) (struct activity *) = {
	wrap_get_cpu_nr,	/* 0 */
	wrap_get_irq_nr,	/* 1 */
	wrap_get_serial_nr,	/* 2 */
	wrap_get_disk_nr,	/* 3 */
	wrap_get_iface_nr,	/* 4 */
	wrap_get_fan_nr,	/* 5 */
	wrap_get_temp_nr,	/* 6 */
	wrap_get_in_nr,		/* 7 */
	wrap_get_usb_nr,	/* 8 */
	wrap_get_filesystem_nr,	/* 9 */
	wrap_get_fchost_nr,	/* 10 */
	wrap_detect_psi,	/* 11 */
	wrap_get_freq_nr,	/* 12 */
	wrap_get_bat_nr		/* 13 */
};
#endif

/*
 * Array of activities.
 * (Order of activities doesn't matter for daily data files).
 */
struct activity *act[NR_ACT] = {
	&cpu_act,
	&pcsw_act,
	&irq_act,
	&swap_act,
	&paging_act,
	&io_act,
	&memory_act,
	&huge_act,
	&ktables_act,
	&queue_act,
	&serial_act,
	&disk_act,
	/* <network> */
	&net_dev_act,
	&net_edev_act,
	&net_nfs_act,
	&net_nfsd_act,
	&net_sock_act,
	&net_ip_act,
	&net_eip_act,
	&net_icmp_act,
	&net_eicmp_act,
	&net_tcp_act,
	&net_etcp_act,
	&net_udp_act,
	&net_sock6_act,
	&net_ip6_act,
	&net_eip6_act,
	&net_icmp6_act,
	&net_eicmp6_act,
	&net_udp6_act,
	&fchost_act,
	&softnet_act,	/* AO_CLOSE_MARKUP */
	/* </network> */
	/* <power-management> */
	&pwr_cpufreq_act,
	&pwr_fan_act,
	&pwr_temp_act,
	&pwr_in_act,
	&pwr_wghfreq_act,
	&pwr_bat_act,
	&pwr_usb_act,	/* AO_CLOSE_MARKUP */
	/* </power-management> */
	&filesystem_act,
	/* <psi> */
	&psi_cpu_act,
	&psi_io_act,
	&psi_mem_act	/* AO_CLOSE_MARKUP */
	/* </psi> */
};
