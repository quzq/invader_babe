/********************************************************************/
/*																	*/
/*	title:	Win32API core											*/
/*	source name:	wincorem.cpp									*/
/*	coded by:	quz													*/
/*	update:	1999/02/10												*/
/*	comment:														*/
/*		win32API��p�����E�C���h�E�쐬�i�l�e�b���g�p���Ȃ��j		*/
/*																	*/
/*																	*/
/*																	*/
/********************************************************************/

/*�R���p�C�� �X�C�b�`*/
#define	SPRITE_ON	/*�X�v���C�g�g�p�r�v*/


/*	�C���N���[�h�t�@�C��	*/
#include	<windows.h>
#include	<ddraw.h>
#include	<stdio.h>
#include	<string.h>
#include	<time.h>	/*�����������Ɏg�p*/
#include	"resource.h"


//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "DDShoot"
#define TITLE               "Direct Draw Shooting Game"
#define	TIMER_ID	1
#define	SPRITE_SURFACE_MAX	2

/*	�v���g�^�C�v�錾	*/
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static	HRESULT	DirectDrawInit(HWND);
static	void	ReleaseAllObjects(void);
static	HRESULT	InitFail(HWND, HRESULT, LPCTSTR,...);
static	void	UpdateFrame(HWND	hWnd);
static	BOOL	RestoreSurfaces(void);
static	void	DrawTopTitle(HWND);
void	FlipEachSurfaces(void);
void	ShootingGameStage(HWND);

/*	�O���[�o���ϐ��錾	*/
char	szClassNme[] = "�V�K�E�B���h�E�N���X��";
BOOL	g_bActive = FALSE;   // Is application active?
int		g_StageHalt;
int		g_StageNum=0;	/*�X�e�[�W��*/


//-----------------------------------------------------------------------------
// Global data	for	DirectDraw
//-----------------------------------------------------------------------------
/*DirectDraw object*/
LPDIRECTDRAW4			g_pDD = NULL;
/*DirectDraw primary surface*/
LPDIRECTDRAWSURFACE4	g_pDDSPrimary = NULL;
/*DirectDraw back surface*/
LPDIRECTDRAWSURFACE4	g_pDDSBack = NULL;
/*DirectDraw�C���[�W�����[�h����X�v���C�g�T�[�t�F�X*/
LPDIRECTDRAWSURFACE4	g_pDDSSprite[2] = { NULL,NULL};
/*DirectDraw�w�i�̃C���[�W�����[�h����o�b�N�O���E���h�T�[�t�F�X */
LPDIRECTDRAWSURFACE4	g_pDDSBackGroundA = NULL;
/*DirectDraw�w�i�L�����N�^�̃C���[�W�����[�h����o�b�N�O���E���h�T�[�t�F�X */
LPDIRECTDRAWSURFACE4	g_pDDSBGCharacterA = NULL;
/*DirectDraw�Q�[�W�p�I�t�X�N���[���T�[�t�F�X */
LPDIRECTDRAWSURFACE4	g_pDDSLevelGage = NULL;
/*DirectDraw Death Spread�p�I�t�X�N���[���T�[�t�F�X */
LPDIRECTDRAWSURFACE4	g_pDDSDeathSpread = NULL;
/*DirectDraw�p���b�g*/
IDirectDrawPalette*		g_pDDPalette=NULL;
/*�p���b�g*/
PALETTEENTRY Pal[256];

/*	Shooting Game Lib	*/
#include	"SGLib.h"
/*snap shot*/
#include	"snapshot.h"


SGLib*	SG=NULL;		/*Shooting Game Lib�I�u�W�F�N�g*/


/****************************************************************************/
/*																			*/
/*		�v�����l�������֐�													*/
/*																			*/
/****************************************************************************/
int	WINAPI	WinMain( HINSTANCE hInstance, HINSTANCE hPreInst,
		LPSTR lpszCmdLine, int nCmdShow )
{
	HWND	hWnd;
	MSG	lpMsg;
	WNDCLASS	myProg;

	if( !hPreInst )/*�C���X�^���X�n���h���̃`�F�b�N(32bit�łł�hPreInst�͏��NULL)*/
	{
		/*WNDCLASS�\���̂̊e�����o�̒�`*/

		/*style:CS_HREDRAW�i���������̃T�C�Y�ύX����������S�̂�`�������j	*/
		/*CS_VREDRAW�i���������̃T�C�Y�ύX�őS�̂�`�������j�Ȃǂ̒萔���w��*/
		myProg.style		=	CS_HREDRAW | CS_VREDRAW;
		/*lpfnWndProc:�E�B���h�E �v���V�[�W��(���b�Z�[�W�̏�������ɍs���֐�)��*/
		myProg.lpfnWndProc	=	WndProc;
		myProg.cbClsExtra	=	0;		/*�������̈�̒ǉ��ݒ�̎��g���B�ʏ�0*/
		myProg.cbWndExtra	=	0;		/*�������̈�̒ǉ��ݒ�̎��g���B�ʏ�0*/
		myProg.hInstance	=	hInstance;				/*�C���X�^���X�̓o�^*/
		myProg.hIcon		=	LoadCursor( NULL, "IDI_ICON1" );;		/*�t���[�� �E�B���h�E�̃A�C�R��*/
		/*hCursor:�}�E�X���E�B���h�E��ɂ���Ƃ��̃J�[�\��*/
		myProg.hCursor		=	LoadCursor( NULL, IDC_ARROW );
		myProg.hbrBackground=	GetStockObject(BLACK_BRUSH);/*�w�i�F(�w�i)�̓o�^*/
		/*lpszMenuName:���j���[�̖��O��o�^�B�Ȃ��ꍇ��NULL*/
		myProg.lpszMenuName	=	NULL;
		/*lpszClassName:�N���X����o�^�B���̃E�B���h�E�Əd���s�B*/
		myProg.lpszClassName=	szClassNme;

		/*���ɂ���CreateWindow��CreateWindowEx�֐����R�[�����邽�߂�*/
		/*�K�v�ȃE�B���h�E�N���X��o�^����							*/
		if( !RegisterClass( &myProg ) )	return( FALSE );
	}
	/*�E�C���h�̐���*/
	/*CreateWindow�֐��̖߂�l�����o�����E�B���h�E�̃E�B���h�E�n���h���ƂȂ�B*/
	/*���̃E�B���h�E�ɑ΂��ĉ�������Ƃ��͂��̃E�B���h�E�n���h�����K�v�ɂȂ�B*/
	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		szClassNme,				/*class name:�o�^����N���X��*/
 		TITLE,/*window name:�^�C�g���o�[�\���L���v�V����*/
		/*window style:�E�B���h�E�X�^�C���ɂ́uWS_���Ƃ��v���w��B*/
		/*�ʏ��WS_OVERLAPPEDWINDOW�w�肵�Ă����Ɗy�B*/
		/*����́A�uWS_���Ƃ��v�������������Ďw�肵���̂Ɠ����B*/
//		WS_OVERLAPPEDWINDOW,	/*window style */
//		CW_USEDEFAULT,			/*horizontal position of window */
//		CW_USEDEFAULT,			/*vertical position of window */
//		CW_USEDEFAULT,			/*window width */
//		CW_USEDEFAULT,			/*window height */
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),

		/*handle to parent or owner window:�e�E�B���h�E�̃n���h���B*/
		/*�q���E�B���h�E�����Ƃ��K�v�B�e�����Ƃ���NULL*/
		NULL,					/*handle to parent or owner window */
		/*handle to menu or child-window identifier:���j���[�n���h���B*/
		/*���j���[��t����Ƃ��K�v�B�N���X���j���[(WNDCLASS�ɓo�^���ꂽ���j���[)*/
		/*���g���Ƃ���NULL*/
		NULL,					/*handle to menu or child-window identifier */
		hInstance,				/*handle to application instance */
		/*pointer to window-creation data:NULL���w�肷��*/
		NULL					/*pointer to window-creation data */
	);
	ShowWindow( hWnd, nCmdShow );	/*�E�C���h�̕\����Ԃ�ݒ�*/
	/*The UpdateWindow function updates the client area						*/
	/*of the specified window by sending a WM_PAINT message					*/
	/*to the window if the window�fs update region is not empty.			*/
	/*The function sends a WM_PAINT message directly 						*/
	/*to the window procedure of the specified window,						*/
	/*bypassing the application queue. If the update region is empty,		*/
	/*no message is sent. */
	UpdateWindow(hWnd);		/*�E�C���h�̍ĕ`��iWM_PAINT���b�Z�[�W�̑��M�j*/

	SetFocus(hWnd);	/*�����E�C���h�Ƀt�H�[�J�X*/


	/*DirectDraw�̏�����*/
    if( DirectDrawInit(hWnd) != DD_OK)	return( FALSE );

	SG=new	SGLib;
	
	/****************************/
	/*	���b�Z�[�W���[�v		*/
	/****************************/

	/*The GetMessage function retrieves a message from the calling			*/
	/*thread�fs message queue and places it in the specified structure.		*/
	/*This function can retrieve both messages 								*/
	/*associated with a specified window and thread messages 				*/
	/*posted via the PostThreadMessage function.							*/
	/*The function retrieves messages that lie within a specified range 	*/
	/*of message values. GetMessage does not retrieve messages for windows 	*/
	/*that belong to other threads or applications. 						*/
	/*���b�Z�[�W�̎擾(WM_QUIT���b�Z�[�W���擾����܂Ń��[�v*/
	while( GetMessage(&lpMsg, NULL, 0, 0) )
	{
	/*The TranslateMessage function translates virtual-key messages			*/
	/*into character messages. The character messages						*/
	/*are posted to the calling thread�fs message queue,					*/
	/*to be read the next time the thread calls the GetMessage				*/
	/*or PeekMessage function												*/
		TranslateMessage(&lpMsg);	/*���b�Z�[�W�̖|��*/
	/*The DispatchMessage function dispatches a message						*/
	/*to a window procedure. It is typically used to dispatch a message		*/
	/*retrieved by the GetMessage function. 								*/
		DispatchMessage(&lpMsg);/*�E�B���h�E�E�v���V�[�W���փ��b�Z�[�W�𑗂�*/
	}

	return( lpMsg.wParam );
}

/****************************************************************************/
/*																			*/
/*		�E�B���h�E �v���V�[�W��(���b�Z�[�W�̏�������ɍs���֐�)			*/
/*																			*/
/****************************************************************************/
LRESULT	CALLBACK	WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case	WM_ACTIVATEAPP:
			// Pause if minimized or not the top window
			g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
			return 0L;

		case	WM_DESTROY:
			// Clean up and close the app
			ReleaseAllObjects();
			delete	SG;
	/*The PostQuitMessage function indicates to Windows 					*/
	/*that a thread has made a request to terminate (quit).					*/
	/* It is typically used in response to a WM_DESTROY message. 			*/
			PostQuitMessage( 0 );
			return 0L;

		case	WM_KEYDOWN:
			// Handle any non-accelerated key commands
			switch (wParam)
			{
			case	VK_F11:
					GetSnapShot(hWnd);
					break;
				case	VK_ESCAPE:
				case	VK_F12:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0L;
				case	VK_SPACE:
					if( g_StageNum == 0 )
					{
						g_StageNum=1;
						g_StageHalt=60;
					}
				case	VK_UP:
				case	VK_DOWN:
					SG->SetStick(1, (UINT) wParam );
					break;
			}
			break;
		case	WM_KEYUP:
			SG->SetStick(0,(UINT) wParam );
			break;
		case	WM_TIMER:
			 // Update and flip surfaces
			if(g_bActive && TIMER_ID == wParam)
			{
				ShootingGameStage(hWnd);
			}
			break;
		default:
	/*The DefWindowProc function calls the default window procedure 		*/
	/*to provide default processing for any window messages 				*/
	/*that an application does not process. This function ensures 			*/
	/*that every message is processed. DefWindowProc is called with 		*/
	/*the same parameters received by the window procedure. 				*/
			return( DefWindowProc(hWnd, msg, wParam, lParam) );
	}
	return( 0L );
}


///////////////////////////////////////////////////////////////////////////
// Create the main DirectDraw object
///////////////////////////////////////////////////////////////////////////
static	HRESULT	DirectDrawInit(HWND hWnd)
{
	HRESULT	hRet;
	int	i;

	/*DirectDraw�I�u�W�F�N�g����*/
	LPDIRECTDRAW	pDD;

	hRet=DirectDrawCreate(NULL, &pDD, NULL);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "DirectDrawCreate FAILED");
	}
	// Fetch DirectDraw4 interface
	hRet=pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *) & g_pDD);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "QueryInterface FAILED");
	}
	// �������x���̐ݒ� Get exclusive mode
	hRet=g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "SetCooperativeLevel FAILED");
	}
	// Set the video mode to 640x480x8
	hRet=g_pDD->SetDisplayMode(640, 480, 8, 0, 0);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "SetDisplayMode FAILED");
	}

	// Create the primary surface with 1 back buffer
	DDSURFACEDESC2	ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
		DDSCAPS_FLIP |
		DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	hRet=g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "CreateSurface FAILED");
	}
    // Get a pointer to the back buffer
	DDSCAPS2	ddscaps;
	ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet=g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");
	}
#ifdef	SPRITE_ON
	/*640*64�X�v���C�g�o�b�t�@�쐬	19990214 quz*/
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = 640;
	ddsd.dwHeight = 64;
	for(i=0;i<SPRITE_SURFACE_MAX;i++)
	{
		hRet=g_pDD->CreateSurface( &ddsd, &g_pDDSSprite[i], NULL );
		if( hRet != DD_OK )
		{
			return InitFail(hWnd, hRet, "CreateSpriteSurface FAILED");
		}
	}
	/*320*32�o�b�N�O���E���h�i�a�f�j�L�����N�^�o�b�t�@�`�쐬	19990216 quz*/
	ddsd.dwWidth = 320;
	ddsd.dwHeight = 32;
	hRet=g_pDD->CreateSurface( &ddsd, &g_pDDSBGCharacterA, NULL );
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "CreateBGCharacterSurface FAILED");
	}

	/*640*480�o�b�N�O���E���h�i�a�f�j�o�b�t�@�`�쐬	19990214 quz*/
	ddsd.dwWidth = 640;
	ddsd.dwHeight = 480;
	hRet=g_pDD->CreateSurface( &ddsd, &g_pDDSBackGroundA, NULL );
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "CreateBackGroundSurface FAILED");
	}

	/*240*8���x���Q�[�W�p�I�t�X�N���[���T�[�t�F�X�쐬	19990214 quz*/
	ddsd.dwWidth = 240;
	ddsd.dwHeight = 8;
	hRet=g_pDD->CreateSurface( &ddsd, &g_pDDSLevelGage, NULL );
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "CreateLevelGageSurface FAILED");
	}

	/*500*50 Death Spread�p�I�t�X�N���[���T�[�t�F�X�쐬	19990220 quz*/
	ddsd.dwWidth = 500;
	ddsd.dwHeight = 50;
	hRet=g_pDD->CreateSurface( &ddsd, &g_pDDSDeathSpread, NULL );
	if( hRet != DD_OK )
	{
		return InitFail(hWnd, hRet, "CreateDeathSpreadSurface FAILED");
	}

	/*�p���b�g�̐���	19990214 quz*/
	g_pDD->CreatePalette( DDPCAPS_8BIT , Pal , &g_pDDPalette , NULL ); 
	g_pDDSPrimary->SetPalette( g_pDDPalette );

	/*�e�T�[�t�F�X�̃��[�h*/
	if( !RestoreSurfaces() )
	{
		ReleaseAllObjects();
		return InitFail(hWnd, hRet, "RestoreSurface FAILED");
	}
#endif

	// Create a timer to flip the pages
	if( TIMER_ID != SetTimer(hWnd, TIMER_ID, 50/*TIMER_RATE*/, NULL) )
	{
		return InitFail(hWnd, hRet, "SetTimer FAILED");
	}
	
	return( DD_OK );
}
//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
static	void	ReleaseAllObjects(void)
{
	if( g_pDD != NULL )
	{
#ifdef	SPRITE_OFF
		if(g_pDDPalette != NULL)
		{
			g_pDDPalette->Release();	/*�p���b�g�̊J��*/
			g_pDDPalette = NULL;
		}
		if( g_pDDSDeathSpread != NULL)
		{
			g_pDDSDeathSpread->Release();	/*Death Spread�̊J��*/
			g_pDDSDeathSpread = NULL;
		}
		if( g_pDDSLevelGage != NULL)
		{
			g_pDDSLevelGage->Release();	/*LevelGage�̊J��*/
			g_pDDSLevelGage = NULL;
		}
		if(g_pDDSBackGroundA != NULL)
		{
			g_pDDSBackGroundA->Release();	/*�a�f�̊J��*/
			g_pDDSBackGroundA = NULL;
		}
		if(g_pDDSBGCharacterA != NULL)
		{
			g_pDDSBGCharacterA->Release();	/*�a�f�L�����N�^�̊J��*/
			g_pDDSBGCharacterA = NULL;
		}
		for( i=0;i<SPRITE_SURFACE_MAX;i++)
		{
			if(g_pDDSSprite[i] != NULL)
			{
				g_pDDSSprite[i]->Release();	/*�X�v���C�g�̊J��*/
				g_pDDSSprite[i] = NULL;
			}
		}
#endif
		if (g_pDDSPrimary != NULL)
		{
			g_pDDSPrimary->Release();/*�v���C�}���T�[�t�F�X�̊J��*/
			g_pDDSPrimary = NULL;
		}
		g_pDD->Release();	/*DirectDraw�I�u�W�F�N�g�̊J��*/
		g_pDD = NULL;
    }
}
// �T�[�t�F�X�̃��[�h
static	BOOL	RestoreSurfaces(void)
{
	HRSRC hRes;
	LPBITMAPINFOHEADER lpBmiHeader;
	RGBQUAD *pRGB;
	HBITMAP hBmp, hBmpOld;
	HDC hDCSrc, hDCDest;
	DDCOLORKEY ddck;
	int	i;
	RECT	rect_to;
	RECT	rect_from;
	DDBLTFX	ddbltfx;

	g_pDDSPrimary->Restore();
	for(i=0;i<SPRITE_SURFACE_MAX;i++)	g_pDDSSprite[i]->Restore();
	g_pDDSBackGroundA->Restore();
	g_pDDSLevelGage->Restore();
	g_pDDSDeathSpread->Restore();

	// �p���b�g���\�[�X��������
	if( (hRes=FindResource( NULL, MAKEINTRESOURCE(IDB_BITMAP1), RT_BITMAP ))==NULL ){
		return FALSE;
	}

	// �p���b�g���\�[�X�̃��[�h
	lpBmiHeader = (LPBITMAPINFOHEADER)LockResource( LoadResource( NULL, hRes ) );
	pRGB = (RGBQUAD*)((char*)lpBmiHeader+lpBmiHeader->biSize);
	if( lpBmiHeader->biClrUsed==0 ) lpBmiHeader->biClrUsed = 1<<lpBmiHeader->biBitCount;

	// �p���b�g���擾
	for( i=0; i<(int)lpBmiHeader->biClrUsed; i++ ){
		Pal[i].peRed   = pRGB[i].rgbRed;
		Pal[i].peGreen = pRGB[i].rgbGreen;
		Pal[i].peBlue  = pRGB[i].rgbBlue;
		Pal[i].peFlags = 0;
	}

	// �p���b�g��ݒ�
	g_pDDPalette->SetEntries( 0, 0, lpBmiHeader->biClrUsed, Pal );

	// ���\�[�X����r�b�g�}�b�v�����[�h
	if( (hBmp = LoadImage( GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP,
		0, 0, LR_CREATEDIBSECTION ))==NULL ){
		return FALSE;
	}

	// �J���[�L�[��ݒ�
	ddck.dwColorSpaceLowValue  = 0; // �����w��
	ddck.dwColorSpaceHighValue = 0;
	
	for(i=0;i<SPRITE_SURFACE_MAX;i++)
	{
		g_pDDSSprite[i]->SetColorKey( DDCKEY_SRCBLT, &ddck ); 
	}
	g_pDDSBGCharacterA->SetColorKey( DDCKEY_SRCBLT, &ddck ); 
	g_pDDSLevelGage->SetColorKey( DDCKEY_SRCBLT, &ddck ); 
	g_pDDSDeathSpread->SetColorKey( DDCKEY_SRCBLT, &ddck ); 

	// �X�v���C�g�o�b�t�@�����O�T�[�t�F�XSprite[0]�փR�s�[����
	hDCSrc = CreateCompatibleDC( NULL );	// �������f�o�C�X�R���e�L�X�g���쐬
	hBmpOld = SelectObject( hDCSrc, hBmp );	// �r�b�g�}�b�v��I��
	g_pDDSSprite[0]->GetDC( &hDCDest );			// �X�v���C�g�T�[�t�F�X��DC���擾
	BitBlt( hDCDest, 0, 0, 32, 32, hDCSrc, 32,0, SRCCOPY ); // �R�s�[
	g_pDDSSprite[0]->ReleaseDC( hDCDest );		// �X�v���C�g�T�[�t�F�X��DC���J��
	SelectObject( hDCSrc, hBmpOld );		// �r�b�g�}�b�v�𕜋A
	DeleteDC( hDCSrc );						// �������f�o�C�X�R���e�L�X�g���J��
	/*Sprite[0]��Sprite[1]�֊g��R�s�[*/
	rect_from.top = 0;
	rect_from.bottom = 32;
	rect_from.left = 0;
	rect_from.right = 160;
	rect_to.top		=	0;
	rect_to.bottom	=	64;
	rect_to.left	=	0;
	rect_to.right	=	320;
	g_pDDSSprite[1]->Blt( &rect_to, g_pDDSSprite[0], &rect_from, DDBLT_WAIT,NULL);
	rect_to.top		=	0;
	rect_to.bottom	=	64;
	rect_to.left	=	320;
	rect_to.right	=	640;
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;	/*���E���]*/
	g_pDDSSprite[1]->Blt( &rect_to, g_pDDSSprite[0], &rect_from, DDBLT_DDFX | DDBLT_WAIT,&ddbltfx);

	// �X�v���C�g�o�b�t�@�����O�T�[�t�F�XSprite[0]�փR�s�[����
	hDCSrc = CreateCompatibleDC( NULL );	// �������f�o�C�X�R���e�L�X�g���쐬
	hBmpOld = SelectObject( hDCSrc, hBmp );	// �r�b�g�}�b�v��I��
	g_pDDSSprite[0]->GetDC( &hDCDest );			// �X�v���C�g�T�[�t�F�X��DC���擾
	BitBlt( hDCDest, 0, 0, 32, 32, hDCSrc, 64,0, SRCCOPY ); // �R�s�[
	g_pDDSSprite[0]->ReleaseDC( hDCDest );		// �X�v���C�g�T�[�t�F�X��DC���J��
	SelectObject( hDCSrc, hBmpOld );		// �r�b�g�}�b�v�𕜋A
	DeleteDC( hDCSrc );						// �������f�o�C�X�R���e�L�X�g���J��
	/*Sprite[0]��Sprite[1]�֊g��R�s�[*/
	rect_from.top = 0;
	rect_from.bottom = 32;
	rect_from.left = 0;
	rect_from.right = 32;
	rect_to.top		=	0;
	rect_to.bottom	=	64;
	rect_to.left	=	320;
	rect_to.right	=	384;
	g_pDDSSprite[1]->Blt( &rect_to, g_pDDSSprite[0], &rect_from, DDBLT_WAIT,NULL);

	// �X�v���C�g�o�b�t�@�����O�T�[�t�F�XSprite[0]�փR�s�[����
	hDCSrc = CreateCompatibleDC( NULL );	// �������f�o�C�X�R���e�L�X�g���쐬
	hBmpOld = SelectObject( hDCSrc, hBmp );	// �r�b�g�}�b�v��I��
	g_pDDSSprite[0]->GetDC( &hDCDest );			// �X�v���C�g�T�[�t�F�X��DC���擾
	BitBlt( hDCDest, 0, 0, 160, 16, hDCSrc, 160,16, SRCCOPY ); // �R�s�[
	g_pDDSSprite[0]->ReleaseDC( hDCDest );		// �X�v���C�g�T�[�t�F�X��DC���J��
	SelectObject( hDCSrc, hBmpOld );		// �r�b�g�}�b�v�𕜋A
	DeleteDC( hDCSrc );						// �������f�o�C�X�R���e�L�X�g���J��
	/*Sprite[0]��Sprite[1]�֊g��R�s�[*/
	rect_from.top = 0;
	rect_from.bottom = 16;
	rect_from.left = 0;
	rect_from.right = 160;
	rect_to.top		=	0;
	rect_to.bottom	=	50;
	rect_to.left	=	0;
	rect_to.right	=	500;
	g_pDDSDeathSpread->Blt( &rect_to, g_pDDSSprite[0], &rect_from, DDBLT_WAIT,NULL);


	hDCSrc = CreateCompatibleDC( NULL );	// �������f�o�C�X�R���e�L�X�g���쐬
	hBmpOld = SelectObject( hDCSrc, hBmp );	// �r�b�g�}�b�v��I��

	g_pDDSBGCharacterA->GetDC( &hDCDest );			// �X�v���C�g�T�[�t�F�X��DC���擾
	BitBlt( hDCDest, 0, 0, 320, 32, hDCSrc, 0,0, SRCCOPY ); // �R�s�[
	g_pDDSBGCharacterA->ReleaseDC( hDCDest );		// �X�v���C�g�T�[�t�F�X��DC���J��

	/*���x���Q�[�W�T�[�t�F�X�ւ̃r�b�g�}�b�v�`��*/
	g_pDDSLevelGage->GetDC( &hDCDest );
	for(i=0;i<240;i+=8)
	{
		BitBlt( hDCDest, i, 0, i+8, 8, hDCSrc, 296, 8, SRCCOPY );
	}
	g_pDDSLevelGage->ReleaseDC( hDCDest );
	
	/*�o�b�N�O���E���h�s�N�`���[�T�[�t�F�X�ւ̃r�b�g�}�b�v�`��*/
	g_pDDSBackGroundA->GetDC( &hDCDest );
	HPEN	hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, 1, RGB(0,0,255));
	hOldPen = SelectObject(hDCDest, hPen);
	int	rx,ry,rr;
	for(i=0;i<199;i++)
	{
		rx=( (int)rand() ) % 640;
		ry=( (int)rand() ) % 480;
		rr=( (int)rand() ) % 4+1;
		Ellipse(hDCDest,rx,ry,rx+rr,ry+rr);
	}
//	Ellipse(hDCDest,100,100,400,400);
	SelectObject(hDCDest, hOldPen);
	DeleteObject(hPen);
//	BitBlt( hDCDest, 0, 0, 320, 32, hDCSrc, 0,0, SRCCOPY ); // �R�s�[
	g_pDDSBackGroundA->ReleaseDC( hDCDest );

	SelectObject( hDCSrc, hBmpOld );		// �r�b�g�}�b�v�𕜋A
	DeleteDC( hDCSrc );						// �������f�o�C�X�R���e�L�X�g���J��


	return TRUE;
}

//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
static	HRESULT	InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
	char	szBuff[128];
	va_list	vl;

	va_start(vl, szError);
	{
		vsprintf(szBuff, szError, vl);
		ReleaseAllObjects();
		MessageBox(hWnd, szBuff, TITLE, MB_OK);
		DestroyWindow(hWnd);
	}
	va_end(vl);
	return( hRet );
}
int	g_Scroll=0;
static	void	UpdateFrame(HWND	hWnd)
{
//	DDBLTFX	ddbltfx;

	// Use the blter to do a color fill to clear the back buffer
	/*�w�i�T�[�t�F�X�̓���*/
//	ddbltfx.dwSize = sizeof(ddbltfx);
//	ddbltfx.dwFillColor = 0;
//	g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
	
	RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/

	/*�퓬�t�B�[���h�w�i*/
	g_Scroll++;
	if(g_Scroll>640)
	{
		g_Scroll=0;
	} else {
		rect_from.top = 0;
		rect_from.bottom = 480;
		rect_from.left = 0;
		rect_from.right = g_Scroll;
		g_pDDSBack->BltFast( 640-g_Scroll, 0, g_pDDSBackGroundA, &rect_from, DDBLTFAST_WAIT);
	}
	rect_from.top = 0;
	rect_from.bottom = 480;
	rect_from.left = g_Scroll;
	rect_from.right = 640;
	g_pDDSBack->BltFast( 0, 0, g_pDDSBackGroundA, &rect_from, DDBLTFAST_WAIT);

	/*�X�v���C�g�̕\��*/
	SG->SpriteDraw();
}

void	ShootingGameStage(HWND	hWnd)
{
	int	game_over_id;
	HDC	hDC;
	int	i;

	if( g_StageNum==0 )
	{
		DDBLTFX	ddbltfx;
		// Use the blter to do a color fill to clear the back buffer
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = 0;
		g_pDDSBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
		if( g_StageHalt>0 )
		{
			g_StageHalt--;
			if(g_pDDSBack->GetDC(&hDC) == DD_OK)
			{
				HFONT	hFont;
				HFONT	hFontOld;

				hFont= CreateFont(
				80,			//�t�H���g����
				0,			//������
				0,		//�e�L�X�g�̊p�x
				0,			//�x�[�X���C���Ƃ����Ƃ̊p�x
				FW_BOLD,	//�t�H���g�̏d���i�����j
				TRUE,		//�C�^���b�N��
				FALSE,		//�A���_�[���C��
				FALSE,		//�ł�������
				SHIFTJIS_CHARSET,	//�����Z�b�g
				OUT_DEFAULT_PRECIS,	//�o�͐��x
				CLIP_DEFAULT_PRECIS,//�N���b�s���O���x
				PROOF_QUALITY,		//�o�͕i��
				FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���[
				"�l�r ����"	//���̖�
				);
				hFontOld = SelectObject(hDC, hFont);
				SetBkColor(hDC, 0);
				SetTextColor(hDC, RGB(255, 0, 0));
				TextOut(hDC,120,180,"GAME OVER",strlen("GAME OVER"));
				SelectObject(hDC, hFontOld);
				DeleteObject(hFont);
				g_pDDSBack->ReleaseDC(hDC);
			}
			FlipEachSurfaces();
			return;
		} else {
			/*�^�C�g�����*/
			DrawTopTitle(hWnd);
			FlipEachSurfaces();
			return;
		}
	}
	if( g_StageHalt==0 )
	{
		game_over_id=SG->Move();
		if( game_over_id==ID_CHR_GROUP_A )	/*�@�̃O���[�v�`�����c�莞*/
		{
			/*�ʃN���A*/
			delete	SG;
			SG=new	SGLib;
			g_StageNum++;
			g_StageHalt=100;
			return;
		} else if ( game_over_id==ID_CHR_GROUP_B || game_over_id==ID_CHR_OTHERS )
		{
			/*�Q�[���I�[�o�[*/
			delete	SG;
			SG=new	SGLib;
			g_StageHalt=100;
			g_StageNum=0;
			return;
		}
	}
	UpdateFrame(hWnd);
	/*�ꎞ��~���b�Z�[�W*/
	if( g_StageHalt >0 )
	{
		g_StageHalt--;
		if(g_pDDSBack->GetDC(&hDC) == DD_OK)
		{
			char	str[20];
			HFONT	hFont;
			HFONT	hFontOld;

			hFont= CreateFont(
			80,			//�t�H���g����
			0,			//������
			0,		//�e�L�X�g�̊p�x
			0,			//�x�[�X���C���Ƃ����Ƃ̊p�x
			FW_BOLD,	//�t�H���g�̏d���i�����j
			FALSE,		//�C�^���b�N��
			FALSE,		//�A���_�[���C��
			FALSE,		//�ł�������
			SHIFTJIS_CHARSET,	//�����Z�b�g
			OUT_DEFAULT_PRECIS,	//�o�͐��x
			CLIP_DEFAULT_PRECIS,//�N���b�s���O���x
			PROOF_QUALITY,		//�o�͕i��
			FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���[
			"�l�r ����"	//���̖�
			);
			hFontOld = SelectObject(hDC, hFont);
			SetBkColor(hDC, 0);
			SetBkMode(hDC,TRANSPARENT);
			SetTextColor(hDC, RGB(0, 0, 0));
			sprintf(str,"STAGE %d",g_StageNum);
			for(i=0;i<9;i++)
			{
				TextOut(hDC,170+(i/3)*3,120+(i%3)*3,str,strlen(str));
				if( g_StageHalt % 16 >8 )
				{
					TextOut(hDC,220+(i/3)*3,230+(i%3)*3,"Ready",strlen("Ready"));
				}
			}
			SetTextColor(hDC, RGB(255, 0, 0));
			TextOut(hDC,170+3,120+3,str,strlen(str));
			if( g_StageHalt % 16 >8 )
			{
				TextOut(hDC,220+3,230+3,"Ready",strlen("Ready"));
			}
			SelectObject(hDC, hFontOld);
			DeleteObject(hFont);
			g_pDDSBack->ReleaseDC(hDC);
		}
	}
	FlipEachSurfaces();
}

void SpriteDebug(void);

static	void	DrawTopTitle(HWND	hWnd)
{
	HDC		hDC;
	RECT	rect_to;
	RECT	rect_from;

	/*�T�[�t�F�X�̕`��p�f�o�C�X�R���e�L�X�g���擾*/
	if(g_pDDSBack->GetDC(&hDC) == DD_OK)
	{
		HBRUSH	hBrush, hOldBrush;
		RECT	rect;
		hBrush = CreateSolidBrush( RGB(127,63,0) );
		hOldBrush = SelectObject(hDC, hBrush);
		for(int i=0;i<40*15;i+=2)
		{
			rect.top=( i % 15 )*32;
			rect.bottom=rect.top+30;
			rect.left=(i / 15)*32;
			rect.right=rect.left+30;
			FillRect(hDC,&rect,hBrush);
		}
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
		HFONT	hFont;
		HFONT	hFontOld;
		hFont= CreateFont(
		80,			//�t�H���g����
		0,			//������
		-330,		//�e�L�X�g�̊p�x
		0,			//�x�[�X���C���Ƃ����Ƃ̊p�x
		FW_BOLD,	//�t�H���g�̏d���i�����j
        FALSE,		//�C�^���b�N��
		FALSE,		//�A���_�[���C��
		FALSE,		//�ł�������
		SHIFTJIS_CHARSET,	//�����Z�b�g
		OUT_DEFAULT_PRECIS,	//�o�͐��x
		CLIP_DEFAULT_PRECIS,//�N���b�s���O���x
		PROOF_QUALITY,		//�o�͕i��
		FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���[
        "�l�r ����"	//���̖�
		);
		hFontOld = SelectObject(hDC, hFont);
		SetBkColor(hDC, 0);
		SetTextColor(hDC, RGB(255, 0, 0));
		TextOut(hDC,320,20,"INVADER",strlen("INVADER"));
		TextOut(hDC,320,120,"BABE",strlen("BABE"));
		SelectObject(hDC, hFontOld);
		DeleteObject(hFont);
		hFont = CreateFont(
		30,			//�t�H���g����
		0,			//������
		0,		//�e�L�X�g�̊p�x
		0,			//�x�[�X���C���Ƃ����Ƃ̊p�x
		FW_BOLD,	//�t�H���g�̏d���i�����j
        FALSE,		//�C�^���b�N��
		FALSE,		//�A���_�[���C��
		FALSE,		//�ł�������
		SHIFTJIS_CHARSET,	//�����Z�b�g
		OUT_DEFAULT_PRECIS,	//�o�͐��x
		CLIP_DEFAULT_PRECIS,//�N���b�s���O���x
		PROOF_QUALITY,		//�o�͕i��
		FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���[
        "�l�r ����"	//���̖�
		);
		hFontOld = SelectObject(hDC, hFont);
		SetTextColor(hDC, RGB(255, 127, 0));
		TextOut(hDC,140,384,"Copyright(C)1999 QWERT",strlen("Copyright(C)1999 QWERT"));
		SelectObject(hDC, hFontOld);
		DeleteObject(hFont);

		hFont = CreateFont(
		20,			//�t�H���g����
		20,			//������
		0,		//�e�L�X�g�̊p�x
		0,			//�x�[�X���C���Ƃ����Ƃ̊p�x
		FW_BOLD,	//�t�H���g�̏d���i�����j
        TRUE,		//�C�^���b�N��
		FALSE,		//�A���_�[���C��
		FALSE,		//�ł�������
		SHIFTJIS_CHARSET,	//�����Z�b�g
		OUT_DEFAULT_PRECIS,	//�o�͐��x
		CLIP_DEFAULT_PRECIS,//�N���b�s���O���x
		PROOF_QUALITY,		//�o�͕i��
		FIXED_PITCH | FF_MODERN,	//�s�b�`�ƃt�@�~���[
        "�l�r ����"	//���̖�
		);
		hFontOld = SelectObject(hDC, hFont);
		SetBkColor(hDC, RGB(0,0,0));
		SetTextColor(hDC, RGB(255, 255, 255));
		if( g_Scroll++>20 ) g_Scroll=0;
		if( g_Scroll>10)
		{
			TextOut(hDC,210,331,"PRESS START",strlen("PRESS START"));
		}
		SelectObject(hDC, hFontOld);
		DeleteObject(hFont);
		g_pDDSBack->ReleaseDC(hDC);
	}
	/*�\�����w�i*/
	rect_from.left	=	320;
	rect_from.right	=	384;
	rect_from.top	=	0;
	rect_from.bottom	=	64;

	rect_to.left	=	100;
	rect_to.right	=	300;
	rect_to.top	=	80;
	rect_to.bottom	=	280;

	g_pDDSBack->Blt( &rect_to,g_pDDSSprite[1], &rect_from, DDBLT_KEYSRC | DDBLT_WAIT ,NULL);
//	SpriteDebug();
}

void	FlipEachSurfaces(void)
{
	/*�T�[�t�F�X�̃t���b�v*/
	while(TRUE)	/*�t���b�v����^�C�~���O��҂�*/
	{
		HRESULT	hRet;
		hRet = g_pDDSPrimary->Flip(NULL, 0);
		if( hRet == DD_OK )	break;		/*����t���b�v�ŒE�o*/
		if( hRet == DDERR_SURFACELOST )	/*�T�[�t�F�X���󎞏���*/
		{
			hRet = g_pDDSPrimary->Restore();
			if ( hRet != DD_OK )	break;
		}
		if( hRet != DDERR_WASSTILLDRAWING )	break;
	}
}

void	SpriteDebug( void )
{
	RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/

	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
	rect_from.top = 0;
	rect_from.bottom = 480;
	rect_from.left = 0;
	rect_from.right = 640;

	g_pDDSBack->BltFast( 0, 0, g_pDDSBackGroundA, &rect_from, DDBLTFAST_WAIT);
}