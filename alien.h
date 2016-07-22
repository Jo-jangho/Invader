#ifndef _ALIEN_H_
#define _ALIEN_H_

/* struct */
typedef struct _S_ALIEN_OBJECT 
{

	int m_nFSM;
	int m_nStep;

	double m_fXpos;
	double m_fYpos;

	double m_fCenterX;
	double m_fCenterY;

	double m_fSpeed;

	_S_MAP_OBJECT *m_pBody;
	_S_BULLET_OBJECT *m_pBullet;

	void (*pfApply)(struct _S_ALIEN_OBJECT *,double);
	void (*pfDraw)(struct _S_ALIEN_OBJECT *,_S_MAP_OBJECT * );

} _S_ALIEN_OBJECT;

/* function */
void alien_init(_S_ALIEN_OBJECT *pObj,_S_MAP_OBJECT *pBody);
static void Draw(_S_ALIEN_OBJECT *pObj, _S_MAP_OBJECT *pBuff);
static void Apply(_S_ALIEN_OBJECT *pObj,double deltaTick);

/* alien init */
static void Apply(_S_ALIEN_OBJECT *pObj,double deltaTick)
{
	/* var */
	double speed = 5.0;
	double lifetime = 3;

	/* run */
	switch(pObj->m_nFSM) 
    {
		case 0: //die
			break;
		case 1: //ready
			if(pObj->m_nStep == 2)
			{	
				pObj->m_nFSM = 2;
			}
			else if(pObj->m_nStep == 3)
			{
				pObj->m_nFSM = 3;
			}
			else if(pObj->m_nStep == 4)
			{
				pObj->m_nFSM = 4;
			}
			else if(pObj->m_nStep == 5)
			{
				pObj->m_nFSM = 5;
			}
			break;
		case 2: //move right
			pObj->m_fXpos += (deltaTick * speed);
			if(pObj->m_fXpos >= 32) 
            {
				pObj->m_nFSM = 3;
				pObj->m_nStep = 3;
			}
			if(pObj->m_pBullet != NULL) 
            {
				if(pObj->m_pBullet->m_nFSM == 0) 
                {
					pObj->m_pBullet->pfFire(pObj->m_pBullet,
							pObj->m_fXpos, pObj->m_fYpos, speed,
							0, 1.0, lifetime);
				}
			}
			break;
		case 3:	//move down
			pObj->m_fYpos += (deltaTick * (speed / 2));
			if(pObj->m_fYpos >= 14) 
            {
				pObj->m_nFSM = 4;
				pObj->m_nStep = 4;
			}
			if(pObj->m_pBullet != NULL) 
            {
				if(pObj->m_pBullet->m_nFSM == 0) 
                {
					pObj->m_pBullet->pfFire(pObj->m_pBullet,
							pObj->m_fXpos, pObj->m_fYpos, speed,
							-1.0, 0, lifetime);
				}
			}
			break;
		case 4: //move left
			pObj->m_fXpos -= (deltaTick * speed);
			if(pObj->m_fXpos <= 2) 
            {
				pObj->m_nFSM = 5;
				pObj->m_nStep = 5;
			}
			if(pObj->m_pBullet != NULL) 
            {
				if(pObj->m_pBullet->m_nFSM == 0) 
                {
					pObj->m_pBullet->pfFire(pObj->m_pBullet,
							pObj->m_fXpos, pObj->m_fYpos, speed,
							0, -1.0, lifetime);
				}
			}
			break;
		case 5: //move up
			pObj->m_fYpos -= (deltaTick * (speed / 2));
			if(pObj->m_fYpos <= 1) 
            {
				pObj->m_nStep = 0;
				pObj->m_nFSM = 2;
			}
			if(pObj->m_pBullet != NULL) 
            {
				if(pObj->m_pBullet->m_nFSM == 0) 
                {
					pObj->m_pBullet->pfFire(pObj->m_pBullet,
							pObj->m_fXpos, pObj->m_fYpos, speed,
							1.0, 0, lifetime);
				}
			}
			break;
	}
}

/* alien draw */
static void Draw(_S_ALIEN_OBJECT *pObj, _S_MAP_OBJECT *pBuff)
{
	switch(pObj->m_nFSM) 
	{
		case 0:
			break;
		default :
			map_drawTile_trn(pObj->m_pBody,
					(int)(pObj->m_fXpos + pObj->m_fCenterX),
					(int)(pObj->m_fYpos + pObj->m_fCenterY),
					pBuff);
			break;
	}
}

/* alien init */
void alien_init(_S_ALIEN_OBJECT *pObj, _S_MAP_OBJECT *pBody)
{
	pObj->m_nFSM = 0;
	pObj->m_nStep = 0;
	pObj->m_pBody = pBody;
	pObj->m_fYpos = 0;
	pObj->m_fXpos = 0;
	pObj->m_fSpeed = 0;
	pObj->m_fCenterX = 0 - (pBody->m_header.m_nWidth/2);
	pObj->m_fCenterY = 0 - (pBody->m_header.m_nHeight/2);

	pObj->m_pBullet = NULL;

	pObj->pfApply = Apply;
	pObj->pfDraw = Draw;
}


#endif