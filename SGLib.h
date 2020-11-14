/****************************************************************/
/*																*/
/*	Shooting Game Lib	SGLib.h									*/
/*																*/
/*	Ver	0.01	1999/02/04	16:50	first fix					*/
/*																*/
/*																*/
/****************************************************************/



#define	USR_SCR_XMIN	0
#define	USR_SCR_XMAX	640
#define	USR_SCR_YMIN	0
#define	USR_SCR_YMAX	400

#define	USR_PLAYER_MAX	(5+16)
#define	USR_PLAYER1	0
#define	USR_PLAYER2	1
#define	USR_SHIP_SIZEX	64
#define	USR_SHIP_SIZEY	64

/*�L�����N�^�O���[�s���O�p�h�c*/
#define	ID_CHR_NOID		0
#define ID_CHR_ALL		1	/*�S�ẴO���[�v�Ɋ܂܂��*/
#define ID_CHR_OTHERS	2	/*�ǂ̃O���[�v�ɂ��܂܂�Ȃ�*/
#define ID_CHR_GROUP_A	16	/*�`�O���[�v�Ɋ܂܂��*/
#define ID_CHR_GROUP_B	17	/*�a�O���[�v�Ɋ܂܂��*/
/*�L�����N�^�����R�[�h*/
#define	ATRB_CODE_SHIP		1
#define	ATRB_CODE_MISSILE	2
#define	ATRB_CODE_SPREAD	3
#define	ATRB_CODE_BLOCK		4

#define	DEV_VIRTUAL_STICK	1	/*�o�[�`�����X�e�B�b�N�f�o�C�X*/

BOOL	SpriteClippingRect(RECT*,RECT*);
BOOL	SpriteClipping(int*,int*,int,int,RECT*);

/****************************************************/
/*													*/
/*	�N���X���C�u����								*/
/*													*/
/****************************************************/
/***********************************/
/*	�t���[�����x���`��֐�			*/
/***********************************/
class	UsrProsessLevel
{
private:
	int	prosess;
public:
	UsrProsessLevel( void ){ prosess=0; }
	void	Draw( HDC hDC )
	{
		char	str[31];
		strcpy(str,"FRAME:");
		if( ++prosess >= 20 )	prosess=0;
		for(int i=0;i<prosess;i++)	str[6+i]='0';
		str[6+i]=0x00;
		TextOut(hDC,10,USR_SCR_YMAX+15,str,strlen(str));
	}
};

/****************************************************/
/*													*/
/*	UsrCtrlDevice�N���X								*/
/*													*/
/*	ver.1.00	1999/01/26							*/
/****************************************************/
class	UsrCtrlDevice
{
private:
	int	up;
	int	down;
	int	left;
	int	right;
	int	btn1;
	int	btn2;
	int	type;	/*0=�s��,1=�X�e�B�b�N,2=����*/
	int	btn1harder;
	int	btn2harder;
public:
	UsrCtrlDevice( int c ){
		type=c;
		up=0;
		down=0;
		left=0;
		right=0;
		btn1=0;
		btn2=0;
	}
	void	SetBtn1( int b ){ btn1=b; }
	void	SetBtn2( int b ){ btn2=b; }
	void	SetUp( int u ){ up=u; }
	void	SetDown( int d ){ down=d; }
	void	SetLeft( int l ){ left=l; }
	void	SetRight( int r ){ right=r; }
	void	SetBtnHarder( int a,int b)
	{
		btn1harder=a;
		btn2harder=b;
	}
	int	GetBtn1Harder( void )
	{
		return(btn1harder);
	}
	int	GetBtn2Harder( void )
	{
		return(btn1harder);
	}
	int	GetPos( void )
	{
		int	pos=0;
		if( down==0 && up==0 )
		{
			pos=4;
		}else if( down ){
			pos=1;
		} else pos=7;
		if( left==0 && right==0 )
		{
			pos+=1;
		} else if( right ) pos+=2;
		if( pos==5 )	pos=0;	/*�L�[�S�I�t����pos=0��*/
		 return( pos );
	}
	int	GetBtn1( void ){ return( btn1 ); }
	int	GetBtn2( void ){ return( btn2 ); }
	int	GetCtrlType(void){ return(type); }
	virtual void	AutoCtrl( int x,int y,int tx,int ty ){}
};


/********************************************************************/
/*																	*/
/*	�N���X���FUsrVirtualPlayerA										*/
/*	����	�F�Ȃ�													*/
/*																	*/
/********************************************************************/
class	UsrVirtualPlayerA:public	UsrCtrlDevice
{
private:
	int	count;	/*������������*/
	int	cnt_max;	/*�����������ԍő�l*/
public:
	UsrVirtualPlayerA( int c, int b1, int b2 ):UsrCtrlDevice( 2 )
	{
		count	=	0;
		cnt_max	=	c;
		SetBtnHarder( b1, b2 );
	}
	void	AutoCtrl( int x,int y,int tx,int ty )
	{
		if( count++ > cnt_max )
		{
			count = 0;
			SetUp( (int)rand() % 3 == 0 );
			SetDown( (int)rand() % 3 == 0 );
		}
		SetBtn1( (int)rand() % GetBtn1Harder() == 0);
		SetBtn2( (int)rand() % GetBtn2Harder() == 0);
	}
};
/********************************************************************/
/*																	*/
/*	�N���X���FUsrVirtualPlayerB										*/
/*	����	�F�Ȃ�													*/
/*																	*/
/********************************************************************/
class	UsrVirtualPlayerB:public	UsrCtrlDevice
{
private:
	int	count;	/*������������*/
	int	cnt_max;	/*�����������ԍő�l*/
public:
	UsrVirtualPlayerB( int c, int b1, int b2 ):UsrCtrlDevice(3)
	{
		count	=	0;
		cnt_max	=	c;
		SetBtnHarder(b1,b2);
	}
	void	AutoCtrl( int x,int y, int tx, int ty )
	{
		if( count++>cnt_max )
		{
			count=0;
			if( y > ty )
			{
				SetUp(1);SetDown(0);
			} else {
				SetUp(0);SetDown(1);
			}
		}
		if( abs(y-ty)<32 )	SetBtn1( (int)rand() % GetBtn1Harder() == 0);
		SetBtn2( (int)rand() % GetBtn2Harder() == 0);
	}
};
/********************************************************************/
/*																	*/
/*	�N���X���FUsrVirtualPlayerC										*/
/*	����	�F�Ȃ�													*/
/*																	*/
/********************************************************************/
class	UsrVirtualPlayerC:public	UsrCtrlDevice
{
private:
	int	count;	/*������������*/
	int	cnt_max;	/*�����������ԍő�l*/
	int	bect;
public:
	UsrVirtualPlayerC( int c,int b1, int b2 ):UsrCtrlDevice(4)
	{
		count	=	0;
		cnt_max	=	c;
		bect=1;
		SetBtnHarder(b1,b2);
	}
	void	AutoCtrl( int x,int y, int tx, int ty )
	{
		if( count++>cnt_max )
		{
			if( y < USR_SCR_YMIN+32 )
			{
				SetUp(0);SetDown(1);
			} else if( y > USR_SCR_YMAX-80 ) {
				SetUp(1);SetDown(0);
			}
			count=0;
		}
		SetBtn1( (int)rand() % GetBtn1Harder() == 0);
		SetBtn2( (int)rand() % GetBtn2Harder() == 0);
	}
};
/********************************************************************/
/*																	*/
/*	�N���X���FUsrVirtualPlayerD										*/
/*	����	�F�Ȃ�													*/
/*																	*/
/********************************************************************/
class	UsrVirtualPlayerD:public	UsrCtrlDevice
{
private:
	int	count;	/*������������*/
	int	cnt_max;	/*�����������ԍő�l*/
	int	bect;
	int	count2;	/*�����Q��������*/
public:
	UsrVirtualPlayerD( int c,int b1, int b2 ):UsrCtrlDevice(4)
	{
		count	=	0;
		cnt_max	=	c;
		bect=-1;
		SetBtnHarder(b1,b2);
		count2=0;
	}
	void	AutoCtrl( int x,int y, int tx, int ty )
	{
		SetBtn1( (int)rand() % GetBtn1Harder() == 0);
		SetBtn2( (int)rand() % GetBtn2Harder() == 0);
		if( count2 > 0 )
		{
			count2--;
			SetLeft(1);
			SetRight(0);
			SetUp(0);
			SetDown(0);
		} else {
			if( count++>cnt_max )
			{
				if( y < USR_SCR_YMIN+8 && bect != 1 )
				{
					bect=1;
					count2=8;
				} else if( y > USR_SCR_YMAX-80 && bect != -1) {
					bect=-1;
					count2=8;
				}
				count=0;
			}
			SetLeft(0);
			SetRight(0);
			SetUp(0);
			SetDown(0);
			if( bect==1 )
			{
				SetDown(1);
			} else if( bect==-1 ) {
				SetUp(1);
			}
		}
	}
};

/****************************************************/
/*													*/
/*	UsrCharacter�N���X								*/
/*													*/
/*	ver.1.00	1999/01/26							*/
/*													*/
/****************************************************/
class	UsrCharacter
{
private:
	int	x;		/*���݂̂w���W*/
	int	y;		/*���݂̂x���W*/
	int	w;		/*�L�����N�^�[�̉���*/
	int	h;		/*�L�����N�^�[�̍���*/
	int	min_x;	/*�ړ��\�Ȃw���W�i�Œ�l�j*/
	int	max_x;	/*�ړ��\�Ȃw���W�i�ő�l�j*/
	int	min_y;	/*�ړ��\�Ȃx���W�i�Œ�l�j*/
	int	max_y;	/*�ړ��\�Ȃx���W�i�ő�l�j*/
	int	group_id;	/*�O���[�s���O�p�h�c*/
	int	attribute;	/*�����i���g�p�j*/
	int	power;		/*�ڐG���U����*/
	int	energy;		/*������*/
	int	time;
	UsrCharacter**	target;
public:
	/*	�R���X�g���N�^	*/
	UsrCharacter(int atr,int	id, int	dx,int	dy,int	dw,int	dh,int	p, int e, int xmin,int xmax,int ymin,int ymax)
	{
		SetXY( dx, dy );
		w=dw;h=dh;
		min_x=xmin;max_x=xmax;
		min_y=ymin;max_y=ymax;
		group_id=id;
		power=p;
		energy=e;
		time=0;
		target=NULL;
		attribute=atr;
	}
	void	AddTime( int t ){	time+=t;	}
	bool	AddXY(int	ax,int	ay)		/*	���W�̈ړ�	*/
	{
		int	dx=x+ax,dy=y+ay;
		if( dx>max_x || dx+w<min_x || dy>max_y || dy+h<min_y )
		{
			return(FALSE);	/*���W�͈͊O�ɏo���ꍇ�̓G���[*/
		} else {
			x=dx;y=dy;
			return(TRUE);	/*���W�͈͓�*/
		}
	}
	bool	DownEnergy( int	damage )	/*�G�l���M�[������������֐�*/
	{
		energy-=damage;		/*������ɃG�l���M�[�����s�����ꍇ�̓G���[*/
		if( energy<=0 )
		{
			energy=0;
			return( FALSE );
		} else return ( TRUE );
	}
	void	DrawEnergy( int x,int y )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/

		int	e=GetEnergy();
		if( e < 1 ) return;
		rect_from.left = 296;
		rect_from.right = rect_from.left+8;
		rect_from.top = 0;
		rect_from.bottom = rect_from.top+8;
		g_pDDSBack->BltFast( x, y, g_pDDSBGCharacterA, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		rect_from.left = 0;
		rect_from.right = rect_from.left+e;
		rect_from.top = 0;
		rect_from.bottom = rect_from.top+8;
		g_pDDSBack->BltFast( x+10, y, g_pDDSLevelGage, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	}
	void	SetTarget( UsrCharacter** t )
	{
		target=t;
	}
	void	SetEnergy( int e ){	energy=e;	}
	void	SetXY( int dx, int dy ){	x=dx;y=dy;	}
	void	SetWH( int dw, int dh ){	w=dw;h=dh;	}
	UsrCharacter**	GetTarget( void){	return( target );	}
	int	GetAttribute( void ){ return( attribute ); }
	int	GetID(void){	return(group_id);	}
	int	GetTime( void ){	return( time );	}
	int	GetX( void ){	return(x);	}
	int	GetY( void ){	return(y);	}
	int	GetWidth( void ){	return(w);	}
	int	GetHeight( void ){	return(h);	}
	int	GetEnergy( void ){	return( energy );	}
	int	GetPower( void ){	return( power );	}
	virtual	bool	Move( void ){ return(0); }
	virtual	void	Draw( void ){}
};
/****************************************************/
/*													*/
/*	UsrMissile�N���X								*/
/*													*/
/****************************************************/
class	UsrMissile:public	UsrCharacter	/*UsrCharacter�N���X���p��*/
{
private:
	int	addx;		/*�w���W�̑����l*/
	int	addy;		/*�x���W�̑����l*/
public:		/*�R���X�g���N�^*/
	UsrMissile( int atr,int id,int x,int y,int w,int h, int ax, int ay, int p, int e )
		:UsrCharacter(atr,id,x,y,w,h,p,e,USR_SCR_XMIN,USR_SCR_XMAX,USR_SCR_YMIN,USR_SCR_YMAX)
	{
		SetBect( ax, ay );
	}
	void	SetParam( int x,int y, int ax, int ay )
	{
		SetXY(x,y);
		SetBect( ax, ay );
	}
	void	SetBect( int ax, int ay ){	addx=ax;addy=ay;	}
	int	GetAY( void ){	return( addy );	}
	int	GetAX( void ){	return( addx );	}
	virtual	bool	Move( void ){ return(TRUE); }
	virtual	void	Draw( void ){}
};

/****************************************************/
/*													*/
/*	UsrDefaultMissile�N���X							*/
/*													*/
/****************************************************/
class	UsrDefaultMissile:public	UsrMissile	/*UsrCharacter�N���X���p��*/
{
private:
public:
	UsrDefaultMissile( int id,int x,int y,int w,int h, int ax, int ay, int p )
		:UsrMissile(ATRB_CODE_MISSILE, id, x, y, w, h, ax, ay, p, 1 ){}
	bool	Move( void )
	{
		if( GetEnergy()<=0 ) return( FALSE );
		return( AddXY( GetAX(), GetAY() ) );
	}
	void	Draw( void )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		int	sx=GetX();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/
		int	sy=GetY();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/

		rect_from.left = 304;
		rect_from.right = rect_from.left+16;
		rect_from.top = ( GetAX()>0 ) ? 0:8;
		rect_from.bottom = rect_from.top+8;

		if( SpriteClipping(&sx,&sy,GetWidth(),GetHeight(),&rect_from) == TRUE )
		{
			g_pDDSBack->BltFast( sx, sy, g_pDDSBGCharacterA, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		}
	}
};

/****************************************************/
/*													*/
/*	UsrDefaultSpread�N���X							*/
/*													*/
/****************************************************/
class	UsrDefaultSpread:public	UsrMissile	/*UsrCharacter�N���X���p��*/
{
private:
public:
	UsrDefaultSpread( int id,int x,int y,int w,int h, int ax, int ay )
		:UsrMissile(ATRB_CODE_SPREAD, id, x, y, w, h, ax, ay, 0, 10 ){}
	bool	Move( void )
	{
		SetXY(GetX()-2, GetY()-2 );
		AddTime(1);
		if( GetTime() >= 10 )
		{
			return( FALSE );
		} else {
			return( AddXY( GetAX(), GetAY() ) );
		}
	}
	void	Draw( void )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		int	sx=GetX();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/
		int	sy=GetY();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/

	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		rect_from.top = 16;
		rect_from.bottom = rect_from.top + 16;
		rect_from.left = 160+GetTime()*16;
		rect_from.right = rect_from.left+16;
		
		if( SpriteClipping(&sx,&sy,GetWidth(),GetHeight(),&rect_from) == TRUE )
		{
			g_pDDSBack->BltFast( sx, sy, g_pDDSBGCharacterA, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		}
	}
};
/****************************************************/
/*													*/
/*	UsrDeathSpread�N���X							*/
/*													*/
/****************************************************/
class	UsrDeathSpread:public	UsrMissile	/*UsrCharacter�N���X���p��*/
{
private:
public:
	UsrDeathSpread( int id,int x,int y,int w,int h, int ax, int ay )
		:UsrMissile( ATRB_CODE_SPREAD,id, x, y, w, h, ax, ay, 0, 10 ){}
	bool	Move( void )
	{
		SetXY(GetX()-2, GetY()-2 );
		AddTime(1);
		if( GetTime() >= 10 )
		{
			return( FALSE );
		} else {
			return( AddXY( GetAX(), GetAY() ) );
		}
	}
	void	Draw( void )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		int	sx=GetX();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/
		int	sy=GetY();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/

	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		rect_from.top = 0;
		rect_from.bottom = rect_from.top + 50;
		rect_from.left = GetTime()*50;
		rect_from.right = rect_from.left+50;
		
		if( SpriteClipping(&sx,&sy,GetWidth(),GetHeight(),&rect_from) == TRUE )
		{
			g_pDDSBack->BltFast( sx, sy, g_pDDSDeathSpread, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		}
	}
};


struct	UsrMsDat
{
	UsrMsDat*	next;
	UsrMsDat*	prev;
	UsrCharacter*	dat;
};


/****************************************************/
/*													*/
/*	UsrChrObjectMgr�N���X							*/
/*													*/
/****************************************************/
class	UsrChrObjectMgr
{
private:
	UsrMsDat*	top;
public:
	UsrChrObjectMgr( void )
	{
		top=new UsrMsDat;
		top->dat=new UsrDefaultSpread(ID_CHR_OTHERS,0,0,0,0,0,0);
		top->next=NULL;
	}
	~UsrChrObjectMgr( void )
	{
		UsrMsDat*	p;
		UsrMsDat*	prev;

		p=top;
		while(p->next!=NULL)
		{
			prev=p;
			p=p->next;
			delete	prev->dat;
			delete	prev;
		}
		delete	p->dat;
		delete	p;
	}
	void	Draw( void )
	{
		UsrMsDat*	p;
		int dx1=50,dy1=USR_SCR_YMAX+2;
		int dx2=50+USR_SCR_XMAX/2,dy2=USR_SCR_YMAX+2;

		p=top->next;
		while( p!=NULL )
		{
			if( p->dat->GetAttribute()==ATRB_CODE_SHIP )
			{
					/*�@�̃G�l���M�[���x���`��*/
				if( p->dat->GetID()==ID_CHR_GROUP_A )
				{
					p->dat->DrawEnergy(dx1,dy1);	
					dy1+=8;
				} else if( p->dat->GetID() == ID_CHR_GROUP_B )
				{
					p->dat->DrawEnergy(dx2,dy2);
					dy2+=8;
				}
			}
			/*�L�����N�^�I�u�W�F�N�g�`��*/
			p->dat->Draw();
			p=p->next;
		}
	}
	void	AddDat( UsrCharacter*	dat)
	{
		UsrMsDat*	p;
		UsrMsDat*	pnew;

		pnew=new UsrMsDat;	/*�V�K�f�[�^�̍쐬*/
		pnew->dat=dat;
		pnew->next=NULL;

		p=top;
		while( p->next!=NULL )	p=p->next;	/*���X�g�f�[�^�Ō���Ɉړ�*/
		p->next=pnew;		/*�V�K�f�[�^�̐ڑ�*/
	}
	int	CheckTouch( int id,int x, int y, int w, int h ,int	dmg)
	{
		UsrMsDat*	p;
		UsrMsDat*	prev;
		int	ret=0;
		prev=top;
		p=top->next;
		while( p!=NULL )
		{
			int	tx=p->dat->GetX();		/*�^�[�Q�b�g�e�̂w���W*/
			int	ty=p->dat->GetY();		/*�^�[�Q�b�g�e�̂x���W*/
			int	tw=p->dat->GetWidth();	/*�^�[�Q�b�g�e�̉���*/
			int	th=p->dat->GetHeight();	/*�^�[�Q�b�g�e�̍���*/
			int	tid=p->dat->GetID();	/*�^�[�Q�b�g�e�̂h�c*/

			if( tid != id && tid != ID_CHR_OTHERS )
			{
				 /*�e�ƈ����̈�Ƃ̐ڐG����*/
				if( abs( x-tx+(w-tw)/2 ) < (w+tw)/2
					&& abs( y-ty+(h-th)/2 ) < (h+th)/2
					&&	p->dat->GetEnergy()>0			)
				{
					AddDat(
						new UsrDefaultSpread(	/*�ڐG���ɂ̓X�v���b�h����*/
							ID_CHR_OTHERS,
							tx+tw/2,
							ty+th/2,
							16,
							16,
							0,
							0
						)
					);
					ret+=p->dat->GetPower();
					p->dat->DownEnergy(dmg);
				}
			}
			prev=p;
			p=p->next;
		}
		return( ret );
	}
	int	Move( void )
	{
		UsrMsDat*	p;
		UsrMsDat*	prev;
		bool	game_over=TRUE;
		int	id_store=ID_CHR_OTHERS;
		int	id_getting=ID_CHR_OTHERS;
		int	ret_id=ID_CHR_OTHERS;

		prev=top;
		p=top->next;
		while( p!=NULL )
		{
			/*���݂���L�����N�^�̎�ʂ𒲍�*/
			id_getting=p->dat->GetID();
			if( id_getting != ID_CHR_OTHERS
				&& id_getting != ID_CHR_ALL
				 )
			{	
				if( id_getting != id_store && id_store != ID_CHR_OTHERS )
				{
					game_over=FALSE;
				}
				id_store=id_getting;
			}

			/*�e�����ݕs�\�̎�*/
			if( p->dat->Move()==FALSE )
			{
				prev->next=p->next;
				delete	p->dat;
				delete	p;
				p=prev;
			}
			prev=p;
			p=p->next;
		}
		if( game_over == TRUE )	return(id_store);
			else return(ID_CHR_NOID);
	}
};

/********************************************************************/
/*																	*/
/*	�N���X���FUsrBlock												*/
/*	����	�Fint	x	�@�̂̍��W									*/
/*			�Fint	y	�@�̂̍��W									*/
/*			�Fint	w	�@�̂̕�									*/
/*			�Fint	h	�@�̂̍���									*/
/*			�Fint	b	�@�̂̌����i�E���P�A�����|�P�j				*/
/*			�Fint	s	�@�̂̈ړ����x								*/
/*			�Fint	mm	�e�}�l�[�W���N���X�̃A�h���X				*/
/*			�Fint	vs	�R���g���[���f�o�C�X�N���X�̃A�h���X		*/
/*						�i�m�t�k�k�ł̓f�o�C�X�Ȃ��j				*/
/*																	*/
/********************************************************************/
class	UsrBlock:public	UsrCharacter	/*UsrCharacter�N���X���p��*/
{
private:
	UsrChrObjectMgr*	msmgr;	/**/
	int	lastcount;	/*���󎞂̃^�C���J�E���^*/
	RECT	data_rect;	/*�r�b�g�}�b�v�i�[�ʒu*/
public:
	UsrBlock( int id,int x,int y,int w,int h, int e, UsrChrObjectMgr* mm )
		:UsrCharacter( ATRB_CODE_BLOCK,id,x, y, w, h, 100, e,USR_SCR_XMIN+w,USR_SCR_XMAX-w,USR_SCR_YMIN+h,USR_SCR_YMAX-h )
	{		/*�R���X�g���N�^*/
		msmgr=mm;
		lastcount=0;
		data_rect.top=0;
		data_rect.bottom=32;
		data_rect.left=0;
		data_rect.right=32;
	}
	bool	Move( void )
	{
		if( GetEnergy()<=0 )
		{
			if( lastcount++ > 99 )
			{
				lastcount = 0;
				return( FALSE );
			}
			/*�@�̕��󎞂̏��� qqq*/
			msmgr->AddDat(
				new UsrDeathSpread(	/*�ڐG���ɂ̓X�v���b�h����*/
					ID_CHR_OTHERS,
					GetX()+( (int)rand()%GetWidth() ),
					GetY()+( (int)rand()%GetHeight() ),
					50,
					50,
					0,
					0
				)
			);
			AddXY( 0, 1 );
			return( TRUE );
		}
		/*�e�Ƃ̐ڐG�`�F�b�N*/
		DownEnergy( msmgr->CheckTouch(GetID(),GetX(),GetY(),GetWidth(),GetHeight(),GetPower() ) );
		return( TRUE );
	}
	void	Draw( void )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/

		/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		rect_from.top = data_rect.top;
		rect_from.bottom = data_rect.bottom;
		rect_from.left = data_rect.left;
		rect_from.right = data_rect.right;

		int	sx=GetX();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/
		int	sy=GetY();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/

		if( SpriteClipping(&sx,&sy,GetWidth(),GetHeight(),&rect_from) == TRUE )
		{
			g_pDDSBack->BltFast( sx, sy, g_pDDSBGCharacterA, &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		}
	}
};

/********************************************************************/
/*																	*/
/*	�N���X���FUsrShotShip											*/
/*	����	�Fint	x	�@�̂̍��W									*/
/*			�Fint	y	�@�̂̍��W									*/
/*			�Fint	w	�@�̂̕�									*/
/*			�Fint	h	�@�̂̍���									*/
/*			�Fint	b	�@�̂̌����i�E���P�A�����|�P�j				*/
/*			�Fint	s	�@�̂̈ړ����x								*/
/*			�Fint	mm	�e�}�l�[�W���N���X�̃A�h���X				*/
/*			�Fint	vs	�R���g���[���f�o�C�X�N���X�̃A�h���X		*/
/*						�i�m�t�k�k�ł̓f�o�C�X�Ȃ��j				*/
/*																	*/
/********************************************************************/
class	UsrShotShip:public	UsrCharacter	/*UsrCharacter�N���X���p��*/
{
private:
	int	bect;	/*�@�̂̌���*/
	int	speed;		/*�@�̂̈ړ����x*/
	UsrChrObjectMgr*	msmgr;	/**/
	UsrCtrlDevice*	stick;	/*�R���g���[���f�o�C�X*/
	int	lastcount;	/*���󎞂̃^�C���J�E���^*/
	RECT	data_rect;	/*�r�b�g�}�b�v�i�[�ʒu*/
	int	OldBtn1;
	int	OldBtn2;
public:
	UsrShotShip( int id,int x,int y,int w,int h, int b, int s, int e, UsrChrObjectMgr* mm, UsrCtrlDevice* cd )
		:UsrCharacter( ATRB_CODE_SHIP,id,x, y, w, h, 100, e,USR_SCR_XMIN+w,USR_SCR_XMAX-w,USR_SCR_YMIN+h,USR_SCR_YMAX-h )
	{		/*�R���X�g���N�^*/
		SetBect(b);
		speed=s;	/*�@�̂̈ړ��X�s�[�h*/
		msmgr=mm;
		stick=cd;
		lastcount=0;
		data_rect.top=0;
		data_rect.bottom=64;
		data_rect.left=( ( b==1 ) ? 0:320 );
		data_rect.right=( ( b==1 ) ? 64:384 );
	}
	void	SetBect( int b ){ bect=b; }
	int	GetBect( void ){ return( bect ); }
	int	GetSpeed( void ){ return( speed ); }
	UsrCtrlDevice*	GetCtrlDev( void )
	{
		return( stick );
	}
	bool	CheckBtn1Enable( void )
	{
		bool	ret	=	FALSE;
		int		btn	=	stick->GetBtn1();
		if( OldBtn1!=btn && btn )	ret=TRUE;
		OldBtn1=btn;
		return( ret );
	}
	bool	Move( void )
	{
		int	pos;
		int	ax=0;		/*�w���W�����l*/
		int	ay=0;		/*�x���W�����l*/

		if( GetEnergy()<=0 )
		{
			if( lastcount++ > 99 )
			{
				lastcount = 0;
				return( FALSE );
			}
			/*�@�̕��󎞂̏��� qqq*/
			msmgr->AddDat(
				new UsrDeathSpread(	/*�ڐG���ɂ̓X�v���b�h����*/
					ID_CHR_OTHERS,
					GetX()+( (int)rand()%GetWidth() ),
					GetY()+( (int)rand()%GetHeight() ),
					50,
					50,
					GetBect()*(-1),
					0
				)
			);
			AddXY( 0, 1 );
			return( TRUE );
		}
		if( stick->GetCtrlType() != DEV_VIRTUAL_STICK )	/*�o�[�`�����v���C���̏ꍇ*/
		{
//			UsrCharacter** target=GetTarget();
//			if( *target == NULL )
//			{
//				for( int i=0;i<USR_PLAYER_MAX;i++ )
//				{
//					if( ships_dat[i]==NULL )	continue;
//					if( ships_dat[i]->GetID() != GetID() 
//						&& ships_dat[i]->GetID() != ID_CHR_OTHERS
//						&& ships_dat[i]->GetID() != ID_CHR_ALL	)
//					{
//						SetTarget( &ships_dat[i] );
//					}
//				}
//			} else {
				/*�X�e�B�b�N�R���g���[���̎����Z�b�g�i�v�l��H�jquz_auto*/
//				stick->AutoCtrl(GetX(),GetY(),(*target)->GetX(),(*target)->GetY());
				stick->AutoCtrl(GetX(),GetY(),0,0);
//			}
		}
		pos=stick->GetPos();
		if( pos>=7 && pos<=9 )
		{
			ay=-speed;
		} else if( pos>=1 && pos<=3 ){
			ay=speed;
		}
		if( pos % 3 ==1 )
		{
			ax=-speed;
		} else if( pos % 3 ==0 && pos != 0 ){
			ax=speed;
		}
		
		AddXY( ax, ay );
		Attack( msmgr );
		/*�e�Ƃ̐ڐG�`�F�b�N*/
		DownEnergy( msmgr->CheckTouch(GetID(),GetX(),GetY(),GetWidth(),GetHeight(),GetPower() ) );
		return( TRUE );
	}
	void	Draw( void )
	{
		RECT	rect_from;	/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/

		/*�I�t�X�N���[���T�[�t�F�X���̈ʒu���*/
		rect_from.top = data_rect.top;
		rect_from.bottom = data_rect.bottom;
		rect_from.left = data_rect.left;
		rect_from.right = data_rect.right;

		int	sx=GetX();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/
		int	sy=GetY();	/*�o�b�N�T�[�t�F�X�ւ̕\���ʒu*/

		if( SpriteClipping(&sx,&sy,GetWidth(),GetHeight(),&rect_from) == TRUE )
		{
			g_pDDSBack->BltFast( sx, sy, g_pDDSSprite[1], &rect_from, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		}
	}
	virtual	void	Attack( UsrChrObjectMgr* msmgr ){};
};



/********************************************************************/
/*																	*/
/*	�N���X���FUsrDefaultShip										*/
/*																	*/
/********************************************************************/
class	UsrDefaultShip:public	UsrShotShip	/*UsrCharacter�N���X���p��*/
{
private:
public:
	UsrDefaultShip( int id,int x, int y, int w, int h, int b, int s, UsrChrObjectMgr* mm, UsrCtrlDevice* cd )
		:UsrShotShip( id, x, y, w, h, b, s, 100, mm, cd ){}
	/*	�R���X�g���N�^	*/
	void	Attack( UsrChrObjectMgr*	msmgr )
	{
		if( CheckBtn1Enable() == TRUE )
		{
			int	msh=8;	/*��������e�̍���*/
			int	msw=16;	/*��������e�̕�*/
			int	msx=GetX();
			if( GetBect() == 1 ) msx+=GetWidth()-msw/2;
			if( GetBect() == -1 ) msx-=msw/2;
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+(GetHeight()-msh)/2,msw,msh, ( GetSpeed() )*GetBect(), 0,10) );
		}
	}
};

/********************************************************************/
/*																	*/
/*	�N���X���FUsrTwinShip											*/
/*																	*/
/********************************************************************/
class	UsrTwinShip:public	UsrShotShip	/*UsrCharacter�N���X���p��*/
{
private:
public:
	UsrTwinShip( int id,int x, int y, int w, int h, int b, int s, UsrChrObjectMgr* mm, UsrCtrlDevice* cd )
		:UsrShotShip( id, x, y, w, h, b, s, 60, mm, cd ){}
	/*	�R���X�g���N�^	*/
	void	Attack( UsrChrObjectMgr*	msmgr )
	{
		if( CheckBtn1Enable() == TRUE )
		{
			int	msh=8;	/*��������e�̍���*/
			int	msw=16;	/*��������e�̕�*/
			int	mswide=16;	/*�e�ƒe�̋߂�*/
			int	msx=GetX();
			if( GetBect() == 1 ) msx+=GetWidth()-msw/2;
			if( GetBect() == -1 ) msx-=msw/2;
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+mswide,msw,msh, ( GetSpeed() )*GetBect(), 0,10) );
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+GetHeight()-msh-mswide,msw,msh, ( GetSpeed() )*GetBect(), 0,10) );
		}
	}
};

/********************************************************************/
/*																	*/
/*	�N���X���FUsr3WayShip											*/
/*																	*/
/********************************************************************/
class	Usr3WayShip:public	UsrShotShip	/*UsrCharacter�N���X���p��*/
{
private:
public:
	Usr3WayShip( int id,int x, int y, int w, int h, int b, int s, UsrChrObjectMgr* mm, UsrCtrlDevice* cd )
		:UsrShotShip( id, x, y, w, h, b, s, 100, mm, cd ){}
	/*	�R���X�g���N�^	*/
	void	Attack( UsrChrObjectMgr*	msmgr )
	{
		if( CheckBtn1Enable() == TRUE )
		{
			int	msh=8;	/*��������e�̍���*/
			int	msw=16;	/*��������e�̕�*/
			int	mswide=8;	/*�e�ƒe�̋߂�*/
			int	msx=GetX();
			if( GetBect() == 1 ) msx+=GetWidth()-msw/2;
			if( GetBect() == -1 ) msx-=msw/2;
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+(GetHeight()-msh)/2,msw,msh, ( GetSpeed() )*GetBect(), -2,3) );
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+(GetHeight()-msh)/2,msw,msh, ( GetSpeed() )*GetBect(), 0,3) );
			msmgr->AddDat( new UsrDefaultMissile( GetID(),msx, GetY()+(GetHeight()-msh)/2,msw,msh, ( GetSpeed()  )*GetBect(), 2,3) );
		}
	}
};

const	int	stage_dat[20]={
	300,225,175,145,125,110,97,85,74,64,
	55,47,40,34,29,25,22,20,18,17
};

class	SGLib
{
private:
	UsrCharacter*	UsrShip[USR_PLAYER_MAX];	/*�v���C���[����2����*/
	UsrChrObjectMgr*	UsrMsMgr;		/*�e�R���g���[���}�l�[�W��*/
	UsrCtrlDevice*	CTRLDev[USR_PLAYER_MAX];
public:
	SGLib(void)
	{
		int apx1=2,apx2=2;
		int	i;

		for(i=0;i<USR_PLAYER_MAX;i++)
		{
			CTRLDev[i]=NULL;
			UsrShip[i]=NULL;
		}
		srand( (unsigned)time(NULL) );	/*�����̏�����*/

		/*�e�R���g���[���}�l�[�W�����̐���*/
		UsrMsMgr=new	UsrChrObjectMgr;
		
		CTRLDev[USR_PLAYER1]=new	UsrCtrlDevice( DEV_VIRTUAL_STICK );	/*�R���g���[���f�o�C�X����*/
		UsrShip[USR_PLAYER1]=new	UsrDefaultShip( ID_CHR_GROUP_A,USR_SCR_XMIN+10, ( USR_SCR_YMAX-USR_SHIP_SIZEY )/2, USR_SHIP_SIZEX, USR_SHIP_SIZEY, 1, 8*2, UsrMsMgr, CTRLDev[USR_PLAYER1] );
		UsrMsMgr->AddDat(UsrShip[USR_PLAYER1]);

		UsrShip[1]=new UsrBlock( ID_CHR_OTHERS,100,100,32,32, 100, UsrMsMgr );
		UsrMsMgr->AddDat(UsrShip[1]);
		UsrShip[2]=new UsrBlock( ID_CHR_OTHERS,116,132,32,32, 100, UsrMsMgr );
		UsrMsMgr->AddDat(UsrShip[2]);
		UsrShip[3]=new UsrBlock( ID_CHR_OTHERS,116,236,32,32, 100, UsrMsMgr );
		UsrMsMgr->AddDat(UsrShip[3]);
		UsrShip[4]=new UsrBlock( ID_CHR_OTHERS,100,268,32,32, 100, UsrMsMgr );
		UsrMsMgr->AddDat(UsrShip[4]);

		int	st_dat=16;	/*default*/
		if( g_StageNum>=0 && g_StageNum<20 ) st_dat=stage_dat[g_StageNum];
		int	apx,apy;
		for(i=5;i<USR_PLAYER_MAX;i++)
		{
			apx=USR_SCR_XMAX-USR_SHIP_SIZEX-( (i-5) % 4 )*( USR_SHIP_SIZEX+USR_SHIP_SIZEX/4 );
			apy=USR_SHIP_SIZEY+USR_SCR_YMIN+( (i-5) / 4 )*( ( USR_SCR_YMAX-USR_SHIP_SIZEY )/4 );
			CTRLDev[i]=new	UsrVirtualPlayerD( 5, st_dat, 0 );	/*quz_auto*/
			UsrShip[i]=new	UsrDefaultShip( ID_CHR_GROUP_B,apx,apy, USR_SHIP_SIZEX, USR_SHIP_SIZEY, -1, 6, UsrMsMgr, CTRLDev[i] );
			UsrMsMgr->AddDat(UsrShip[i]);
//			UsrShip[i]->SetTarget(&UsrShip[USR_PLAYER1]);
		}
//		for(;i<USR_PLAYER_MAX;i++)
//		{
//			CTRLDev[i]=new	UsrVirtualPlayerA( 5, 10, 15 );	/*quz_auto*/
//			UsrShip[i]=new	UsrTwinShip( ID_CHR_GROUP_A,USR_SCR_XMIN+10+(apx2++)*USR_SHIP_SIZEX/2, 0, USR_SHIP_SIZEX, USR_SHIP_SIZEY, 1, 4*2, UsrMsMgr, CTRLDev[i] );
//			UsrShip[i]->SetTarget(&UsrShip[USR_PLAYER2]);
//		}
	}
	~SGLib(void)
	{
		for(int i=0;i<USR_PLAYER_MAX;i++)
		{
			if( CTRLDev[i] != NULL )	delete CTRLDev[i];
		}
		delete	UsrMsMgr;
	}
	void	SetStick(int sw,UINT nChar)
	{
		for(int i=0;i<USR_PLAYER_MAX;i++)
		{
			if( CTRLDev[i] == NULL || CTRLDev[i]->GetCtrlType() != DEV_VIRTUAL_STICK )	continue;
			switch( nChar )
			{
				case	VK_UP:	/*��L�[����*/
					CTRLDev[i]->SetUp(sw);
					break;
				case	VK_DOWN:	/*���L�[����*/
					CTRLDev[i]->SetDown(sw);
					break;
				case	VK_LEFT:	/*���L�[����*/
					CTRLDev[i]->SetLeft(sw);
					break;
				case	VK_RIGHT:	/*�E�L�[����*/
					CTRLDev[i]->SetRight(sw);
					break;
				case	VK_SPACE:	/*�r�o�b�L�[����*/
					CTRLDev[i]->SetBtn1(sw);
					break;
				default:
					break;
			}
		}
	}
	BOOL	Move(void)
	{
		bool	game_over=TRUE;
		int	id_store=ID_CHR_OTHERS;
		int	id_getting=ID_CHR_OTHERS;
		int	ret_id=ID_CHR_OTHERS;

		return( UsrMsMgr->Move() );	/*�e�ړ�*/
	}
	void	SpriteDraw(void)
	{
		UsrMsMgr->Draw();		/*�e�`��*/
	}
};


/*�T�|�[�g�֐��Q*/
		// �N���b�s���O	�T�|�[�g
BOOL	SpriteClipping(int*	x,int*	y,int w,int h,RECT* rect )
{
	if( *x<USR_SCR_XMIN )
	{
		rect->left -= ( *x-USR_SCR_XMIN );
		*x = 0;
	}
	if( *x+w>USR_SCR_XMAX )
	{
		rect->right -= ( *x+w-USR_SCR_XMAX );
	}
	if( *y<USR_SCR_YMIN )
	{
		rect->top -= ( *y-USR_SCR_YMIN );
		*y = 0;
	}
	if( *y+h>USR_SCR_YMAX )
	{
		rect->bottom -= ( *y+h-USR_SCR_YMAX );
	}
	if( rect->left >= rect->right ) return(FALSE);
	if( rect->top >= rect->bottom ) return(FALSE);

	return(TRUE);
}

BOOL	SpriteClippingRect(RECT* rect_to,RECT* rect_from )
{
	int	width_to=rect_to->right-rect_to->left;
	int	width_from=rect_from->right-rect_from->left;
	int	height_to=rect_to->bottom-rect_to->top;
	int	height_from=rect_from->bottom-rect_from->top;

	if( rect_to->left<USR_SCR_XMIN )
	{
		rect_from->left -= ( rect_to->left-USR_SCR_XMIN )*width_from/width_to;
		rect_to->left = 0;
	}
	if( rect_to->right>USR_SCR_XMAX )
	{
		rect_from->right -= ( rect_to->right-USR_SCR_XMAX )*width_from/width_to;
		rect_to->right=USR_SCR_XMAX;
	}
	if( rect_to->top<USR_SCR_YMIN )
	{
		rect_from->top -= ( rect_to->top-USR_SCR_YMIN )*height_from/height_to;
		rect_to->top=0;
	}
	if( rect_to->bottom>USR_SCR_YMAX )
	{
		rect_from->bottom -= ( rect_to->bottom-USR_SCR_YMAX )*height_from/height_to;
		rect_to->bottom=USR_SCR_YMAX;
	}
	if( rect_from->left >= rect_from->right ) return(FALSE);
	if( rect_from->top >= rect_from->bottom ) return(FALSE);

	return(TRUE);
}

