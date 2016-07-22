#ifndef __TEMP_1_H__
#define __TEMP_1_H__

/* define */
#define WIDTH 35
#define HEIGHT 16

/* struct */
typedef struct _S_Plane
{
    int m_nPosX;
    int m_nPosY;
    _S_MAP_OBJECT *m_pBody;

    void (*pfApply)(struct _S_Plane *, double, char );
    void (*pfDraw)(struct _S_Plane *, _S_MAP_OBJECT *);
}_S_Plane;

/* var */
struct timespec work_timer;
double acc_tick, last_tick;
double score_acc_tick;
int bLoop = 1;
int score = 0;
int nFSM = 0;
int nStep = 0;

int mis_posX;
int mis_posY;

/* function */
void Plane_init(_S_Plane *pObj, _S_MAP_OBJECT *pBody, int x, int y);
void Plane_Apply(_S_Plane *pObj, double deltaTick, char key_input);
void Plane_Draw(_S_Plane *pObj, _S_MAP_OBJECT *pBuff);

/* game object */
_S_MAP_OBJECT gMap;
_S_MAP_OBJECT gScreenBuffer;
_S_MAP_OBJECT gPlayer;
_S_MAP_OBJECT gMissile;
_S_MAP_OBJECT gAlienModel;

_S_Plane gPlayerPlane;
_S_BULLET_OBJECT gBullets[32];
_S_ALIEN_OBJECT gAlienObjects[8];

/* player init */
void Plane_init(_S_Plane *pObj, _S_MAP_OBJECT *pBody, int x, int y)
{
    pObj->m_pBody = pBody;
    pObj->m_nPosX = x;
    pObj->m_nPosY = y;

    pObj->pfApply = Plane_Apply;
    pObj->pfDraw = Plane_Draw;
}

/* player apply */
void Plane_Apply(_S_Plane *pObj, double deltaTick, char key_input)
{  
    switch(key_input)
    {
        case 'w':   // up
            if(pObj->m_nPosY > 0)
            {
                pObj->m_nPosY--;
            }
            break;
        case 'a':   // left
            if(pObj->m_nPosX > 0)
            {
                pObj->m_nPosX--;
            }
            break;
        case 's':   // down
            if(pObj->m_nPosY < HEIGHT - 3)
            {
                pObj->m_nPosY++;
            }
            break;
        case 'd':   // right
            if(pObj->m_nPosX < WIDTH - 5)
            {
                pObj->m_nPosX++;
            }
            break;
    }
}

/* player draw */
void Plane_Draw(_S_Plane *pObj, _S_MAP_OBJECT *pBuff)
{
    map_drawTile_trn(pObj->m_pBody, pObj->m_nPosX, pObj->m_nPosY, pBuff);
}

#endif