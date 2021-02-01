
	.globl		_pcg_roll90
	.globl		_bgpcg_roll90
	.globl		pcg_roll90
	.globl		bgpcg_roll90


	.include	doscall.mac
	.include	iocscall.mac


*==========================================================================
*
*	�X�^�b�N�t���[���̍쐬
*
*==========================================================================

	.offset 0

arg1_l	ds.b	2
arg1_w	ds.b	1
arg1_b	ds.b	1

arg2_l	ds.b	2
arg2_w	ds.b	1
arg2_b	ds.b	1

arg3_l	ds.b	2
arg3_w	ds.b	1
arg3_b	ds.b	1

arg4_l	ds.b	2
arg4_w	ds.b	1
arg4_b	ds.b	1


	.text
	.even


*==========================================================================
*
*	�����Fvoid pcg_roll90(void *pcg, int lr);
*
*	  IN : a0.l = *pcg
*	       d0.l = lr
*
*	  OUT: ����
*
*	BREAK: ����
*
*--------------------------------------------------------------------------

_pcg_roll90:

A7ID	=	4+9*4			*   �X�^�b�N�� return��A�h���X  [ 4 byte ]
					* + �ޔ����W�X�^�̑S�o�C�g��     [ 9*4 byte ]


	movea.l	4+arg1_l(sp),a0		* a0.l = ��]���� PCG �̃A�h���X
	move.l	4+arg2_l(sp),d0		* d0.l = ��]����
					* ���W�X�^�ޔ�O�ɓǂݏo���̂ŃI�t�Z�b�g�ɒ���

*-------[ ������ ]
pcg_roll90:
	movem.l	d0-d3/d6-d7/a1-a3,-(sp)	* ���W�X�^�ޔ�

	lea.l	temp_pcg,a1		* a1.l = ���������f�[�^�̈ꎞ�����ݐ�

	tst.l	d0
	bmi.b	L90
	beq	EXIT


*-------[ �E��] ]
R90:
	lea.l	(a0),a2
	lea.l	64(a1),a3
	bsr	ROLL_R

	lea.l	32(a0),a2
	lea.l	(a1),a3
	bsr	ROLL_R

	lea.l	64(a0),a2
	lea.l	96(a1),a3
	bsr	ROLL_R

	lea.l	96(a0),a2
	lea.l	32(a1),a3
	bsr	ROLL_R

	bra.b	@F


*-------[ ����] ]
L90:
	lea.l	(a0),a2
	lea.l	32(a1),a3
	bsr	ROLL_L

	lea.l	32(a0),a2
	lea.l	96(a1),a3
	bsr	ROLL_L

	lea.l	64(a0),a2
	lea.l	(a1),a3
	bsr	ROLL_L

	lea.l	96(a0),a2
	lea.l	64(a1),a3
	bsr	ROLL_L


*-------[ �ꎞ�����ݐ���R�s�[ ]
@@:
	movem.l	(a1)+,d0-d3/d6-d7/a2-a3
	movem.l	d0-d3/d6-d7/a2-a3,(a0)
	movem.l	(a1)+,d0-d3/d6-d7/a2-a3
	movem.l	d0-d3/d6-d7/a2-a3,4*8(a0)
	movem.l	(a1)+,d0-d3/d6-d7/a2-a3
	movem.l	d0-d3/d6-d7/a2-a3,4*16(a0)
	movem.l	(a1)+,d0-d3/d6-d7/a2-a3
	movem.l	d0-d3/d6-d7/a2-a3,4*24(a0)


*-------[ �I�� ]
EXIT:
	movem.l	(sp)+,d0-d3/d6-d7/a1-a3	* ���W�X�^����
	rts




*==========================================================================
*
*	�����Fvoid bgpcg_roll90(void *pcg, int lr);
*
*	  IN : a0.l = *pcg
*	       d0.l = lr
*
*	  OUT: ����
*
*	BREAK: ����
*
*--------------------------------------------------------------------------

_bgpcg_roll90:

A7ID	=	4+9*4			*   �X�^�b�N�� return��A�h���X  [ 4 byte ]
					* + �ޔ����W�X�^�̑S�o�C�g��     [ 9*4 byte ]


	movea.l	4+arg1_l(sp),a0		* a0.l = ��]���� PCG �̃A�h���X
	move.l	4+arg2_l(sp),d0		* d0.l = ��]����
					* ���W�X�^�ޔ�O�ɓǂݏo���̂ŃI�t�Z�b�g�ɒ���

*-------[ ������ ]
bgpcg_roll90:
	movem.l	d0-d3/d6-d7/a1-a3,-(sp)	* ���W�X�^�ޔ�

	lea.l	temp_pcg,a1		* a1.l = ���������f�[�^�̈ꎞ�����ݐ�
	lea.l	(a0),a2
	lea.l	(a1),a3

	tst.l	d0
	bmi.b	BG_L90
	beq.b	BG_EXIT


*-------[ �E��] ]
BG_R90:
	bsr	ROLL_R
	bra.b	@F


*-------[ ����] ]
BG_L90:
	bsr	ROLL_L


*-------[ �ꎞ�����ݐ���R�s�[ ]
@@:
	movem.l	(a1)+,d0-d3/d6-d7/a2-a3
	movem.l	d0-d3/d6-d7/a2-a3,(a0)


*-------[ �I�� ]
BG_EXIT:
	movem.l	(sp)+,d0-d3/d6-d7/a1-a3	* ���W�X�^����
	rts




*==========================================================================
*
*	8x8 �h�b�g�͈͂̉E 90 �x��]
*
*	�����Fa2.l = �Ǐo���� ��`����[�A�h���X
*	      a3.l = �����ݐ� ��`����[�A�h���X
*
*	�j��Fd0-d3/d6-d7/a2-a3
*
*--------------------------------------------------------------------------

ROLL_R:
	lea.l	3(a3),a3
	moveq	#3,d7

ROLL_R_YLOOP:
		moveq	#3,d6

ROLL_R_XLOOP:
			move.b	(a2),d0		* d0.b = [0][1]
			move.b	4(a2),d1	* d1.b = [2][3]

			move.b	d0,d2
			lsr.b	#4,d2		* d2.b = [ ][0]
			move.b	d1,d3
			andi.b	#$F0,d3		* d3.b = [2][ ]
			or.b	d3,d2		* d2.b = [2][0]

			andi.b	#$0F,d0		* d0.b = [ ][1]
			lsl.b	#4,d1		* d1.b = [3][ ]
			or.b	d1,d0		* d0.b = [3][1]

			move.b	d2,(a3)		* [2][0] �]��
			move.b	d0,4(a3)	* [3][1] �]��

			lea.l	1(a2),a2
			lea.l	8(a3),a3

		dbra	d6,ROLL_R_XLOOP

		lea.l	4(a2),a2
		lea.l	-32-1(a3),a3

	dbra	d7,ROLL_R_YLOOP

	rts




*==========================================================================
*
*	8x8 �h�b�g�͈͂̍� 90 �x��]
*
*	�����Fa2.l = �Ǐo���� ��`����[�A�h���X
*	      a3.l = �����ݐ� ��`����[�A�h���X
*
*	�j��Fd0-d3/d6-d7/a2-a3
*
*--------------------------------------------------------------------------

ROLL_L:
	lea.l	24(a3),a3
	moveq	#3,d7

ROLL_L_YLOOP:
		moveq	#3,d6

ROLL_L_XLOOP:
			move.b	(a2),d0		* d0.b = [0][1]
			move.b	4(a2),d1	* d1.b = [2][3]

			move.b	d0,d2
			lsl.b	#4,d2		* d2.b = [1][ ]
			move.b	d1,d3
			andi.b	#$0F,d3		* d3.b = [ ][3]
			or.b	d3,d2		* d2.b = [1][3]

			andi.b	#$F0,d0		* d0.b = [0][ ]
			lsr.b	#4,d1		* d1.b = [ ][2]
			or.b	d1,d0		* d0.b = [0][2]

			move.b	d2,(a3)		* [1][3] �]��
			move.b	d0,4(a3)	* [0][2] �]��

			lea.l	1(a2),a2
			lea.l	-8(a3),a3

		dbra	d6,ROLL_L_XLOOP

		lea.l	4(a2),a2
		lea.l	32+1(a3),a3

	dbra	d7,ROLL_L_YLOOP

	rts




*==========================================================================
*
*	�������m��
*
*==========================================================================

	.bss
	.even

temp_pcg:	ds.b	128


