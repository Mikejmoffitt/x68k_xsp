/*
	XSP ���p�T���v���v���O����

	1) �e�튄�荞�ݏ����̎��s
	2) �P�̃X�v���C�g�̕\��
	3) �����X�v���C�g�̕\��
	4) PCM8A �Ƃ̊��荞�ݏՓ˂̉��
	�� 4 �_���s�Ȃ��Ă��܂��B

	�v���O�������N������Ƃ��낢��q�˂Ă��܂����A�������͂������̂܂܃�
	�^�[���L�[�������Əȗ������ƌ��Ȃ���A�f�t�H���g�l���ݒ肳��܂��B

	���l�̓��͂��ς܂���ƁA��ʏ���w�薇���̃p�l�����������f�����n��
	��܂��i���̔����͒P�̃X�v���C�g�̃p�l���A�c��̔����͕����X�v���C�g
	�̃p�l���ł��j�B�p�l���ɏ�����Ă���ԍ��͕\���D��x��\���܂��B�D��
	�x�j�]�y�����[�h�ł́A�قȂ�D��x�̃p�l���Ԃ̕\���D��x���ی삳���
	���i����D��x�̃p�l���Ԃ̕\���D��x�͕ی삳��܂���j�BXSP_MODE ��
	�f�t�H���g�l�� 3�A�܂�D��x�j�]�y�����[�h�ł��B����� 2 ���w�肷
	��ƁA�D��x�j�]�y�����s���Ȃ��Ȃ�܂��B3 ���w�肵���ꍇ�� 2 ���w
	�肵���ꍇ�̓���̈Ⴂ���A�p�l���Ԃ̕\���D��x�ɒ��ڂ��Ȃ���m�F����
	�݂ĉ������B

	�܂��APCM8A �g�p���ł��Փ˂��邱�ƂȂ����삵�Ă��邱�Ƃ��m�F���邽��
	PCM8A �풓���APCM �h�����𑽃`�����l���p�����Ȃ����t������Ԃł����s
	���Ă݂ĉ������B���̏ꍇ�A�~���[�W�b�N�h���C�o�[�ɂ� MCDRV �� ZMUSIC
	��p���AZMUSIC �ł͏풓���ɃX�C�b�` -M ���w�肵�ă��X�^���荞�݂���
	���ĉ������BMXDRV �̓��X�^���荞�݂������Ă��Ȃ��̂Ŏg�p�s�ł��B

	��ʍ��[�ɕ\������Ă�����́A�X�v���C�g�_�u���[�����̃��X�^������
	�u�������Ă��܂��B�N�����ɁA���X�^���� Y ���W���������� on ���w�肵
	���ꍇ�A�\�����̑������C����T�����ē��I�ɍX�V�����l�q����̈ʒu
	����m�F�ł��܂��B
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <doslib.h>
#include <iocslib.h>
#include <graph.h>
#include <math.h>
#include "../../XSP/XSP2lib.H"
#include "../../PCM8Afnc/PCM8Afnc.H"

int		input2(char *mes, int def);
void	ras_scroll();
void	ras_scroll_init();

/* �X�v���C�g PCG �p�^�[���ő�g�p�� */
#define	PCG_MAX		256


/*------------------------------------[ XSP �֘A ]------------------------------------*/

/*
	XSP �p PCG �z�u�Ǘ��e�[�u��
	�X�v���C�g PCG �p�^�[���ő�g�p�� + 1 �o�C�g�̃T�C�Y���K�v�B
*/
char	pcg_alt[PCG_MAX + 1];

/* PCG �f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */
char	pcg_dat[PCG_MAX * 128];

/* XSP �����X�v���C�g�t���[���f�[�^ */
XOBJ_FRM_DAT	frm_dat[512];

/* XSP �����X�v���C�g���t�@�����X�f�[�^ */
XOBJ_REF_DAT	ref_dat[512];

/* ���[�U�[�����X�^���荞�݃^�C���`���[�g */
XSP_TIME_CHART	time_chart[512];

/* �p���b�g�f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */
unsigned short	pal_dat[256];


/*------------------------[ ���X�^�X�N���[���p sin �e�[�u�� ]-------------------------*/

/* sin �e�[�u�� */
short	wave[256];

/* sin �e�[�u���ǂݏo���ʒu�̃C���f�b�N�X */
short	wave_ptr = 0;

/* sin �e�[�u���ǂݏo���ʒu�̃C���f�b�N�X�̏����l */
short	wave_ptr_00 = 0;


/*-----------------------------[ �L�����N�^�Ǘ��\���� ]-------------------------------*/

/* 512 ���̃L�����N�^�������\���� */
struct {
	short	x, y;		/* ���W */
	short	pt;			/* �X�v���C�g�p�^�[�� No. */
	short	info;		/* ���]�R�[�h�E�F�E�D��x��\���f�[�^ */
	short	vx, vy;		/* �ړ��� */
	int		dummy;		/* �������̂��ߍ\���̃T�C�Y���p�f�B���O */
} SP[512];


/*-------------------------------------[ MAIN ]---------------------------------------*/
void main()
{
	int		i, j;
	int		panel_max;
	int		mode;
	int		crt;
	int		adjust_divy;
	int		min_divh;
	int		ras_int;
	FILE	*fp;


	mode		= input2("	XSP_MODE ", 3);
	crt			= input2("	CRT_MODE [1]=31Khz : [2]=15Khz ", 1);
	adjust_divy	= input2("	���X�^���� Y ���W��������  [1]=ON : [2]=OFF ", 1);
	min_divh	= input2("	���X�^�����u���b�N�c���ŏ��l ", 32);
	ras_int		= input2("	���X�^�X�N���[��  [1]=ON : [2]=OFF ", 1);
	panel_max	= input2("	�p�l������ ", 32);
	if (panel_max > 512) panel_max = 512;


	/*---------------------[ ��ʂ������� ]---------------------*/

	if (crt == 1) {
		/* 256x256 dot 16 �F�O���t�B�b�N�v���[�� 4 �� 31KHz */
		CRTMOD(6);
	} else {
		/* 256x256 dot 16 �F�O���t�B�b�N�v���[�� 4 �� 15KHz */
		CRTMOD(7);
	}

	/* �O���t�B�b�N�\�� ON */
	G_CLR_ON();

	/* �X�v���C�g�\���� ON */
	SP_ON();

	/* BG0 �\�� OFF */
	BGCTRLST(0, 0, 0);

	/* BG1 �\�� OFF */
	BGCTRLST(1, 1, 0);

	/* �O���t�B�b�N�p���b�g 1 �Ԃ�^�����ɂ��� */
	GPALET(1, 0xFFFF);

	/* �J�[�\���\�� OFF */
	B_CUROFF();


	/*------------------[ PCG �f�[�^�ǂݍ��� ]------------------*/

	fp = fopen("../PANEL.SP", "rb");
	fread(
		pcg_dat,
		128,		/* 1PCG = 128byte */
		256,		/* 256PCG */
		fp
	);
	fclose(fp);


	/*--------[ �X�v���C�g�p���b�g�f�[�^�ǂݍ��݂ƒ�` ]--------*/

	fp = fopen("../PANEL.PAL", "rb");
	fread(
		pal_dat,
		2,			/* 1color = 2byte */
		256,		/* 16color * 16block */
		fp
	);
	fclose(fp);

	/* �X�v���C�g�p���b�g�ɓ]�� */
	for (i = 0; i < 256; i++) {
		SPALET((i & 15) | (1 << 0x1F), i / 16, pal_dat[i]);
	}


	/*-----------[ �����X�v���C�g�̌`��f�[�^���쐬 ]-----------*/

	j = 0;

	for (i = 0; i < 16; i++) {
		ref_dat[i].num	= 16;			/* �����X�v���C�g�� */
		ref_dat[i].ptr	= &frm_dat[j];	/* �Q�Ƃ���t���[���f�[�^�ւ̃|�C���^ */

		frm_dat[j].vx	= -0x18;
		frm_dat[j].vy	= -0x18;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;


		frm_dat[j].vx	= -0x30;
		frm_dat[j].vy	=  0x10;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;


		frm_dat[j].vx	= -0x30;
		frm_dat[j].vy	=  0x10;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;


		frm_dat[j].vx	= -0x30;
		frm_dat[j].vy	=  0x10;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;

		frm_dat[j].vx	=  0x10;
		frm_dat[j].vy	=  0;
		frm_dat[j].pt	=  i;
		frm_dat[j].rv	=  0;
		j++;
	}


	/*---------------------[ XSP �������� ]---------------------*/

	/* XSP �̏����� */
	xsp_on();

	/* ���샂�[�h�ݒ� */
	xsp_mode(mode);

	/* ���X�^���� Y ���W�̎������� */
	if (adjust_divy == 1) {
		xsp_auto_adjust_divy(1);
		xsp_min_divh_set(min_divh);
	} else {
		xsp_auto_adjust_divy(0);
	}

	/* PCG �f�[�^�� PCG �z�u�Ǘ����e�[�u�����w�� */
	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));

	/* �����X�v���C�g�`��f�[�^���w�� */
	xsp_objdat_set(ref_dat);

	/* PCM8A �Ƃ̏Փ˂���� */
	pcm8a_vsyncint_on();


	/*--------------[ �L�����N�^ 512 �������� ]---------------*/

	for (i = 0; i < 512; i++) {
		SP[i].x		= ((rand() / 16 & 255) + 8) * 64;	/* X ���W������ */
		SP[i].y		= ((rand() / 16 & 255) + 8) * 64;	/* Y ���W������ */
		SP[i].pt	= i & 7;							/* �X�v���C�g�p�^�[�� No. �������� */
		SP[i].info	= 0x138 + (i & 7) * 0x101;			/* �J���[ 1�A�D��x 0x38 �` 0x3F */
		SP[i].vx	= (rand() / 16 & 127) - 64;			/* X �����ړ��� */
		SP[i].vy	= (rand() / 16 & 127) - 64;			/* Y �����ړ��� */
	}


	/*------[ ���[�U�[�����X�^���荞�݃^�C���`���[�g�쐬 ]------*/

	i = 0;
	for (j = 0; j < 256; j += 4) {
		time_chart[i].ras_no	= j * 2 + 32;
		time_chart[i].proc		= ras_scroll;
		i++;
	}

	time_chart[i].ras_no = -1;		/* �G���h�}�[�N */


	/*----------[ ���X�^�X�N���[���p sin �e�[�u���쐬 ]---------*/

	for (i = 0; i < 256; i++) {
		wave[i] = sin( 3.1415926535898 * (double)i / 128 ) * 64;
	}


	/*-------------------[ ���荞�ݏ����ݒ� ]-------------------*/

	if (ras_int == 1) {
		/* �t�H���g�\�� */
		symbol(0, 0x10, "���X�^�X�N���[����", 1, 4, 2, 1, 0);
		symbol(0, 0x90, "�X�v���C�g�_�u������", 1, 4, 2, 1, 0);

		/* �A�����Ԋ��荞�݊J�n */
		xsp_vsyncint_on(ras_scroll_init);

		/* ���X�^���荞�݊J�n */
		xsp_hsyncint_on(time_chart);
	}


	/*============================[ �L���������ˉ��f�� ]==============================*/

	while (INPOUT(0xFF) == 0) {
		/* �������� */
		xsp_vsync(1);

		/* �����͒P�̃X�v���C�g�ŕ\�� */
		for (i = 0; i < panel_max / 2; i++) {
			SP[i].x += SP[i].vx;
			SP[i].y += SP[i].vy;
			xsp_set(SP[i].x >> 6, SP[i].y >> 6, SP[i].pt, SP[i].info);
			if (SP[i].x <= 0 || 0x110 * 64 <= SP[i].x) SP[i].vx =- SP[i].vx;
			if (SP[i].y <= 0 || 0x110 * 64 <= SP[i].y) SP[i].vy =- SP[i].vy;
		}

		/* �����͕����X�v���C�g�ŕ\�� */
		for (i = panel_max / 2; i < panel_max; i++) {
			SP[i].x += SP[i].vx;
			SP[i].y += SP[i].vy;
			xobj_set(SP[i].x >> 6, SP[i].y >> 6, SP[i].pt, SP[i].info);
			if (SP[i].x <= 0 || 0x110 * 64 <= SP[i].x) SP[i].vx =- SP[i].vx;
			if (SP[i].y <= 0 || 0x110 * 64 <= SP[i].y) SP[i].vy =- SP[i].vy;
		}

		/* ���X�^���� Y ���W�ɖ��\���i�f�o�b�O�p�j*/
		xsp_set(16, xsp_divy_get(0), 9, 0x13F);
		xsp_set(16, xsp_divy_get(1), 9, 0x13F);
		xsp_set(16, xsp_divy_get(2), 9, 0x13F);
		xsp_set(16, xsp_divy_get(3), 9, 0x13F);
		xsp_set(16, xsp_divy_get(4), 9, 0x13F);
		xsp_set(16, xsp_divy_get(5), 9, 0x13F);
		xsp_set(16, xsp_divy_get(6), 9, 0x13F);

		/* �X�v���C�g���ꊇ�\������ */
		xsp_out();
	}


	/*-----------------------[ �I������ ]-----------------------*/

	/* XSP �̏I������ */
	xsp_off();

	/* ��ʃ��[�h��߂� */
	CRTMOD(0x10);
}


/*
	�f�t�H���g�l�L��� input() �֐�
*/
int input2(
	char *mes,	/* ���b�Z�[�W */
	int def		/* �f�t�H���g�l */
){
	char	str[35];
	int		tmp;

	printf("%s[ �ȗ�=%d ]=", mes, def);
	str[0] = 32;
	tmp = atoi(cgets(str));
	printf("\n");
	if (tmp == 0) tmp = def;
	return(tmp);
}


/*
	�A�����Ԋ��荞�݃T�u���[�`��
*/
void ras_scroll_init()
{
	wave_ptr	= wave_ptr_00;
	wave_ptr_00	=(wave_ptr_00 + 1) & 255;
}


/*
	���X�^���荞�݃T�u���[�`��
*/
void ras_scroll()
{
	*(short *)(0xE80018) = wave[wave_ptr];	/* �O���t�B�b�N�v���[��0 X���W */
	wave_ptr = (wave_ptr + 1) & 255;
}


