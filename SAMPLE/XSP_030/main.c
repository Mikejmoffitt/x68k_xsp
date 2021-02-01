/*
	XSP ���p�T���v���v���O����

	CVOBJ.X ���o�͂��镡���X�v���C�g�`��f�[�^�i�g���q .xsp .frm .ref�j
	��ǂݍ��ݕ\������v���O�����ł��B�W���C�X�e�B�b�N�ŕ����X�v���C�g��
	�������A�g���K�ŎQ�ƃp���b�g�ƃp�^�[����ύX���邱�Ƃ��ł��܂��i����
	���ő�����\�j�B�����X�v���C�g�̊ȈՉ{���c�[���Ƃ��Ă��g���܂��B

	�{�v���O�����ł́A�X�v���C�g�p���b�g�̏������͍s���Ă��Ȃ��̂ŁA�K�v
	�ȏꍇ�͎��O�ɑ��̃c�[����p����Ȃǂ��ď��������s���ĉ������B

	ZAKO.xsp/frm/ref ���A�{�v���O�����Ή��̃T���v���f�[�^�ł��BZAKO.SP ��
	PCG �f�[�^�AZAKO.src �͕����X�v���C�g�`��f�[�^�\�[�X�t�@�C���ł��B
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "../../XSP/XSP2lib.H"

FILE *chk_open(char *fname, char *mode);

/* �X�v���C�g PCG �p�^�[���ő�g�p�� */
#define		PCG_MAX		2048

/* �����X�v���C�g���t�@�����X�f�[�^�ő�g�p�� */
#define		REF_MAX		4096

/* �����X�v���C�g�t���[���f�[�^�ő�g�p�� */
#define		FRM_MAX		16384


/*
	XSP �p PCG �z�u�Ǘ��e�[�u��
	�X�v���C�g PCG �p�^�[���ő�g�p�� + 1 �o�C�g�̃T�C�Y���K�v�B
*/
char	pcg_alt[PCG_MAX + 1];

/* PCG �f�[�^�t�@�C���ǂݍ��݃o�b�t�@ */
char	pcg_dat[PCG_MAX * 128];

/* XSP �����X�v���C�g�t���[���f�[�^ */
XOBJ_FRM_DAT	frm_dat[FRM_MAX];

/* XSP �����X�v���C�g���t�@�����X�f�[�^ */
XOBJ_REF_DAT	ref_dat[REF_MAX];


/*-------------------------------------[ MAIN ]---------------------------------------*/
void main(int argc, unsigned char* argv[])
{
	int		i;
	int		sizeof_ref;		/* �����X�v���C�g���t�@�����X�f�[�^�ǂݍ��ݐ� */
	FILE	*fp;

	/* �L�����N�^�Ǘ��\���� */
	struct {
		short	x, y;		/* ���W */
		short	pt;			/* �X�v���C�g�p�^�[�� No. */
		short	info;		/* ���]�R�[�h�E�F�E�D��x��\���f�[�^ */
	} MYCHARA;


	/*----------[ �R�}���h���C����́`�t�@�C���ǂݍ��� ]----------*/

	if (argc <= 1) {
		/* �w���v��\�����ďI�� */
		printf("�g�p�@�Fmain.X [�`��f�[�^�t�@�C�����i�g���q�ȗ��j]\n");
		exit(0);
	} else {
		char	str_tmp[256];

		/* �t�@�C���ǂݍ��� */
		strmfe(str_tmp, argv[1], "xsp");		/* �g���q�u�� */
		fp = chk_open(str_tmp, "rb");
		fread(pcg_dat, sizeof(char), 128 * PCG_MAX, fp);

		strmfe(str_tmp, argv[1], "frm");		/* �g���q�u�� */
		fp = chk_open(str_tmp, "rb");
		fread(frm_dat, sizeof(XOBJ_FRM_DAT), FRM_MAX, fp);

		strmfe(str_tmp, argv[1], "ref");		/* �g���q�u�� */
		fp = chk_open(str_tmp, "rb");
		sizeof_ref = fread(ref_dat, sizeof(XOBJ_REF_DAT), REF_MAX, fp);

		fcloseall();

		/* REF_DAT[].ptr �␳ */
		for (i = 0; i < sizeof_ref; i++) {
			(int)ref_dat[i].ptr += (int)(&frm_dat[0]);
		}
	}


	/*---------------------[ ��ʂ������� ]---------------------*/

	/* 256x256 dot 16 �F�O���t�B�b�N�v���[�� 4 �� 31KHz */
	CRTMOD(6);

	/* �X�v���C�g�\���� ON */
	SP_ON();

	/* BG0 �\�� OFF */
	BGCTRLST(0, 0, 0);

	/* BG1 �\�� OFF */
	BGCTRLST(1, 1, 0);

	/* �J�[�\���\�� OFF */
	B_CUROFF();


	/*---------------------[ XSP �������� ]---------------------*/

	/* XSP �̏����� */
	xsp_on();

	/* PCG �f�[�^�� PCG �z�u�Ǘ����e�[�u�����w�� */
	xsp_pcgdat_set(pcg_dat, pcg_alt, sizeof(pcg_alt));

	/* �����X�v���C�g�`��f�[�^���w�� */
	xsp_objdat_set(ref_dat);


	/*==========================[ �X�e�B�b�N�ő��삷��f�� ]============================*/

	/* ������ */
	MYCHARA.x		= 0x88;		/* X ���W�����l */
	MYCHARA.y		= 0x88;		/* Y ���W�����l */
	MYCHARA.pt		= 0;		/* �X�v���C�g�p�^�[�� No. */
	MYCHARA.info	= 0x013F;	/* ���]�R�[�h�E�F�E�D��x��\���f�[�^ */


	/* �����L�[�������܂Ń��[�v */
	while (INPOUT(0xFF) == 0) {
		static int	pre_stk = 0;
		static int	stk = 0;
		static int	timer = 0;

		/* �������� */
		xsp_vsync(1);

		/* �X�e�B�b�N�̓��͂ɍ����Ĉړ� */
		pre_stk = stk;		/* �O��̃X�e�B�b�N�̓��e */
		stk = JOYGET(0);	/* ����̃X�e�B�b�N�̓��e */
		if ((stk & 1) == 0) MYCHARA.y -= 1;		/* ��Ɉړ� */
		if ((stk & 2) == 0) MYCHARA.y += 1;		/* ���Ɉړ� */
		if ((stk & 4) == 0) MYCHARA.x -= 1;		/* ���Ɉړ� */
		if ((stk & 8) == 0) MYCHARA.x += 1;		/* �E�Ɉړ� */

		/* �g���K���������ԑ��� */
		if ((stk & 0x60) == (pre_stk & 0x60)) {
			timer++;		/* �^�C�}���Z */
		} else {
			timer = 0;		/* �^�C�}�N���A */
		}

		/* �g���K�[ 2 �������ꂽ��p�^�[���ύX */
		if ((stk & 0x20) == 0  &&  ((pre_stk & 0x20) != 0  ||  timer > 32)) {
			MYCHARA.pt++;
			if (MYCHARA.pt >= sizeof_ref) MYCHARA.pt = 0;
		}

		/* �g���K�[ 1 �������ꂽ��F�ύX */
		if ((stk & 0x40) == 0  &&  ((pre_stk & 0x40) != 0  ||  timer > 32)) {
			if ((MYCHARA.info & 0x0F00) == 0x0F00) {
				MYCHARA.info &= 0xF0FF;		/* �J���[�R�[�h�� 0 �ɖ߂� */
			} else {
				MYCHARA.info += 0x100;		/* �J���[�R�[�h�� 1 ���Z */
			}
		}

		/* pt info ����ʂɕ\�� */
		B_LOCATE(0, 0);
		printf("  pt = %3X \n", MYCHARA.pt);
		printf("info = %3X \n", MYCHARA.info);

		/* �X�v���C�g�̕\���o�^ */
		xobj_set(MYCHARA.x, MYCHARA.y, MYCHARA.pt, MYCHARA.info);
		/*
			�������́A
			xobj_set_st(&MYCHARA);
			�ƋL�q����΁A��荂���Ɏ��s�ł���B
		*/

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
	�t�@�C�����G���[���o�@�\�t���� fopen() �֐�
*/
FILE *chk_open(
	char *fname,	/* �t�@�C���� */
	char *mode		/* ���[�h */
){
	FILE	*fp;

	fp = fopen(fname, mode);
	if (fp == (FILE*)0) {
		fcloseall();
		printf("\n");
		printf("	%s �� open �ł��܂���ł����B\n", fname);
		printf("	�����I�����܂��B\n\n");
		printf("	�i�����L�[�������ĉ������B�j\n");
		while (INPOUT(0xFF) == 0) {}
		exit(0);
	}

	return(fp);
}


