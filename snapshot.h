/********************************************/
/*	DirectDraw	SnapShotLiblaly				*/
/********************************************/

void	GetSnapShot(HWND hWnd)
{
	HBITMAP hBmp, hBmpOld;
	HDC hDC_to, hDC_from;
	HWND	hWnd_snap;


	if(g_pDDSPrimary->GetDC(&hDC_from) != DD_OK)
	{
		ReleaseAllObjects();
		DestroyWindow(hWnd);
		return;
	} else {
		// �r�b�g�}�b�v�̏����̂��߂ɉ�ʃT�C�Y�𓾂�B
		int nScrnW=GetSystemMetrics(SM_CXSCREEN);
		int nScrnH=GetSystemMetrics(SM_CYSCREEN);


		nScrnW=640;
		nScrnH=480;

//		ShowWindow(SW_HIDE);

		// ��ʂɌ݊����̂���DC�����B
		hDC_to=CreateCompatibleDC(hDC_from);
		// �r�b�g�}�b�v�̈�����B
		hBmp=CreateCompatibleBitmap(hDC_from,nScrnW,nScrnH);
		// �쐬�����̈�ɉ�ʂ�]�ʂ���B
		SelectObject(hDC_to,hBmp);
		BitBlt(hDC_to,0,0,nScrnW,nScrnH,hDC_from,0,0,SRCCOPY);

		// �N���b�v�{�[�h���J��
		OpenClipboard(NULL);
//		// �N���b�v�{�[�h����ɂ���
		EmptyClipboard();
//		// �r�b�g�}�b�v���N���b�v�{�[�h�ɑ���
		SetClipboardData(CF_BITMAP,hBmp);
//		// �N���b�v�{�[�h�����
		CloseClipboard();

		// �s�v�ɂȂ������̂��폜
		DeleteDC(hDC_to);
		g_pDDSPrimary->ReleaseDC(hDC_from);
//		ShowWindow(SW_SHOW); // ��

		ReleaseAllObjects();
		DestroyWindow(hWnd);

		return;
	}
}