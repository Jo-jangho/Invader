#ifndef __BULLET_H__
#define __BULLET_H__

/* struct */
typedef struct _S_BULLET_OBJECT
{
	int m_nFSM;
	int m_nStep;

	double m_fPosX;
	double m_fPosY;

    double m_fvx;
    double m_fvy;

	double m_fSpeed;

	double m_faccLifeTime; //현재 생존시간 
	double m_fLifeLimit; //수명

	_S_MAP_OBJECT *m_pBody;

    void (*pfApply)(struct _S_BULLET_OBJECT *, double);
    void (*pfDraw)(struct _S_BULLET_OBJECT *, struct _S_MAP_OBJECT *);
    void (*pfFire)(struct _S_BULLET_OBJECT *, int , int , double , double , double , double );
    void (*pfVector)(struct _S_BULLET_OBJECT *, double , double , double , double );

}_S_BULLET_OBJECT;


/* function */
void bullet_apply(_S_BULLET_OBJECT *pObj, double deltaTick);
void bullet_draw(_S_BULLET_OBJECT *pObj, _S_MAP_OBJECT *pMapBuf);
void bullet_fire(_S_BULLET_OBJECT *pObj, int x, int y, double speed, double vx, double vy, double lifeLimit);
void bullet_vector(_S_BULLET_OBJECT *pObj, double startX, double startY, double endX, double endY);

/* bullet init */
void bullet_init(_S_BULLET_OBJECT *pObj, double x, double y, double speed, _S_MAP_OBJECT *pBody)
{
    pObj->m_nFSM = 0;   //  0 : sleep,  1 : active
    pObj->m_nStep = 0;
    pObj->m_fSpeed = speed;
    pObj->m_fPosX = x;
    pObj->m_fPosY = y;
    pObj->m_fLifeLimit = 0;
    pObj->m_pBody = pBody;

    pObj->pfApply = bullet_apply;
    pObj->pfDraw = bullet_draw;
    pObj->pfFire = bullet_fire;
    pObj->pfVector = bullet_vector;
}

/* bullet apply */
void bullet_apply(_S_BULLET_OBJECT *pObj, double deltaTick)
{
	switch(pObj->m_nFSM) 
    {
		case 0:
		    break;
		case 1:
            if(pObj->m_nStep == 0) 
            {
                pObj->m_nStep++;
                pObj->m_faccLifeTime = 0;
            }
            else 
            {
                pObj->m_faccLifeTime += deltaTick; //수명처리 
                if(pObj->m_faccLifeTime > pObj->m_fLifeLimit) 
                {
                    pObj->m_nStep = 0;
                    pObj->m_nFSM = 0;
                }
                pObj->m_fPosY += deltaTick * (pObj->m_fSpeed * pObj->m_fvy);
                pObj->m_fPosX += deltaTick * (pObj->m_fSpeed * pObj->m_fvx);
            }
		    break;
	}
}

/* bullet draw */
void bullet_draw(_S_BULLET_OBJECT *pObj, _S_MAP_OBJECT *pMapBuf)
{
    switch(pObj->m_nFSM) 
    {
        case 0:
            break;
        case 1:
            map_drawTile_trn(pObj->m_pBody,
            (int)pObj->m_fPosX,
            (int)pObj->m_fPosY,
            pMapBuf);
            break;
	}
}

/* bullet fire */
void bullet_fire(_S_BULLET_OBJECT *pObj, int x, int y, double speed, double vx, double vy, double lifeLimit)
{
	pObj->m_nFSM = 1;
	pObj->m_nStep = 0;
	pObj->m_fPosX = (double)x;
	pObj->m_fPosY = (double)y;
    pObj->m_fvx = vx;
    pObj->m_fvy = vy;
	pObj->m_fSpeed = speed;
	pObj->m_fLifeLimit = lifeLimit;
}

/* bullet vector*/
void bullet_vector(_S_BULLET_OBJECT *pObj, double startX, double startY, double endX, double endY)
{
    double mx = startX;
    double my = startY;

    double vx = endX - startX;
    double vy = endY - startY;
    double c = sqrt((vx * vx) + (vy * vy));
    vx /= c;
    vy /= c;

    bullet_fire(pObj, startX, startY, 1.0, vx, vy, 3.0);  
}

#endif