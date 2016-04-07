/* start.S
 * Heiher <r@hev.cc>
 */

#define CORE_BUF_BASE	0x900000003ff01000

	.set	mips64r2
	.set	noreorder

	.text

	.align	2
	.global	_start
	.ent	_start
	.type	_start, @function
_start:
	mfc0	$t0, $12
	li	$t1, 7
	ins	$t0, $t1, 28, 3
	ori	$t0, 0x80
	mtc0	$t0, $12

	bal	1f
	nop
	.word	_gp
1:
	lw	$gp, 0($ra)
	dla	$sp, _sp0

	dli	$t0, CORE_BUF_BASE
	sd	$zero, 0x38($t0)
	sync

	# core 1
	dli	$t0, CORE_BUF_BASE + 0x100
	dla	$t1, _sp1
	sd	$t1, 0x28($t0)
	sd	$gp, 0x30($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)
	# core 2
	dli	$t0, CORE_BUF_BASE + 0x200
	dla	$t1, _sp2
	sd	$t1, 0x28($t0)
	sd	$gp, 0x30($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)
	# core 3
	dli	$t0, CORE_BUF_BASE + 0x300
	dla	$t1, _sp3
	sd	$t1, 0x28($t0)
	sd	$gp, 0x30($t0)
	sync
	dla	$t1, _slave_start
	sd	$t1, 0x20($t0)

	dla	$t9, master_main
	jalr	$t9
	nop

	b	_start
	nop
	.end	_start

	.global	_slave_start
	.ent	_slave_start
	.type	_slave_start, @function
_slave_start:
	mfc0	$t0, $12
	li	$t1, 7
	ins	$t0, $t1, 28, 3
	ori	$t0, 0x80
	mtc0	$t0, $12

	mfc0	$t0, $15, 1
	andi	$t0, 0x3ff
	andi	$s0, $t0, 0x3	# local cpuid

	dla	$t9, slave_main
	jalr	$t9
	move	$a0, $s0

_slave_wait:
	dli	$t0, CORE_BUF_BASE
	sll	$s0, 8
	or	$t0, $s0

	sd	$zero, 0x20($t0)
1:
	ld	$t9, 0x20($t0)
	beqz	$t9, 1b
	nop
	ld	$sp, 0x28($t0)
	ld	$gp, 0x30($t0)
	jalr	$t9
	nop
	.end	_slave_start

	.data

	.align 3
	.space	0x10000
_sp0:
	.space	0x10000
_sp1:
	.space	0x10000
_sp2:
	.space	0x10000
_sp3:
