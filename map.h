#ifndef __MAP_H__
#define __MAP_H__

/* struct */
typedef struct
{
	int m_nSkima;
	int m_nWidth;
	int m_nHeight;
}_S_MAP_HEADER;

typedef struct _S_MAP_OBJECT
{
	_S_MAP_HEADER m_header;
	char *m_pBuf;

    int (*fpLoad)(struct _S_MAP_OBJECT *, char *);
    int (*fpSave)(struct _S_MAP_OBJECT *, char *);
    void (*fpDump)(struct _S_MAP_OBJECT *);
    void (*fpNew)(struct _S_MAP_OBJECT *, int , int );
    void (*fpPut)(struct _S_MAP_OBJECT *, int , int , int );
    void (*fpHline)(struct _S_MAP_OBJECT *, int , int );
    void (*fpVline)(struct _S_MAP_OBJECT *, int , int );
    void (*fpDrawTile)(struct _S_MAP_OBJECT *, int , int , struct _S_MAP_OBJECT *);
    void (*fpDrawTile_trn)(struct _S_MAP_OBJECT *, int , int , struct _S_MAP_OBJECT *);
    void (*fpDrawTile_MirrorV)(struct _S_MAP_OBJECT *, int , int , struct _S_MAP_OBJECT *);
    void (*fpDrawTile_MirrorH)(struct _S_MAP_OBJECT *, int , int , struct _S_MAP_OBJECT *);
}_S_MAP_OBJECT;

/* function */
void map_dump(_S_MAP_OBJECT *pObj);
void map_new(_S_MAP_OBJECT *pObj, int nWidth, int nHeight);
void map_put(_S_MAP_OBJECT *pObj, int x, int y, int tile_index);
void map_hline(_S_MAP_OBJECT *pObj, int x, int tile_index);
void map_vline(_S_MAP_OBJECT *pObj, int y, int tile_index);
void map_drawTile(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget);
void map_drawTile_trn(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget);
void map_drawTile_mirrorV(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget);
void map_drawTile_mirrorH(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget);
int map_save(_S_MAP_OBJECT *pObj, char *filename);
int map_load(_S_MAP_OBJECT *pObj, char *filename);

/* var */
char tilePalette[] = {'.', '#', '@', '/', 92, 
                        '+', '=', '^', '>', '<', 
                        '|', '-', '*'};

/* map init */
void map_init(_S_MAP_OBJECT *pObj)
{
    pObj->m_pBuf = NULL;

    pObj->fpLoad = map_load;
    pObj->fpSave = map_save;
    pObj->fpDump = map_dump;
    pObj->fpNew = map_new;
    pObj->fpPut = map_put;
    pObj->fpHline = map_hline;
    pObj->fpVline = map_vline;
    pObj->fpDrawTile = map_drawTile;
    pObj->fpDrawTile_trn = map_drawTile_trn;
    pObj->fpDrawTile_MirrorV = map_drawTile_mirrorV;
    pObj->fpDrawTile_MirrorH = map_drawTile_mirrorH;
}

/* all map draw */
void map_dump(_S_MAP_OBJECT *pObj)
{
    for(int iy = 0 ; iy < pObj->m_header.m_nHeight ; iy++) 
    {
		for(int ix = 0; ix < pObj->m_header.m_nWidth ; ix++) 
        {
			putchar(tilePalette[ pObj->m_pBuf[ iy * pObj->m_header.m_nWidth + ix]]);
		}
		printf("\r\n");
	}
}

/* map new */
void map_new(_S_MAP_OBJECT *pObj, int nWidth, int nHeight)
{
    //new 8 4
    if(pObj->m_pBuf != NULL) 
    {
        free(pObj->m_pBuf); 
    }
    pObj->m_header.m_nWidth = nWidth;
    pObj->m_header.m_nHeight = nHeight;
    pObj->m_pBuf = malloc( nHeight * nWidth );

    for(int i = 0 ; i <	nHeight * nWidth ; i++) 
    {
        pObj->m_pBuf[i] = 0;
    }
}

/* dot push */
void map_put(_S_MAP_OBJECT *pObj, int x, int y, int tile_index)
{
    pObj->m_pBuf[ (y * pObj->m_header.m_nWidth) + x ] = tile_index;
}

/* height line push */
void map_hline(_S_MAP_OBJECT *pObj, int x, int tile_index)
{
    for(int i = 0 ; i < pObj->m_header.m_nHeight ; i++)
    {
        pObj->m_pBuf[ (i * pObj->m_header.m_nWidth) + x ] = tile_index;
    }
}

/* vertical line push*/
void map_vline(_S_MAP_OBJECT *pObj, int y, int tile_index)
{
    for(int i = 0 ; i < pObj->m_header.m_nWidth ; i++)
    {
        pObj->m_pBuf[ (y * pObj->m_header.m_nWidth) + i ] = tile_index;
    }
}

/* written map1 */
void map_drawTile(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget)
{
    for( int i = 0 ; i < pObj->m_header.m_nHeight ; i++)
    {
        for(int j = 0 ; j < pObj->m_header.m_nWidth ; j++)
        {
            map_put(pTarget, j + posX, i + posY, 
            pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j]);
        }
    }
}

/* written map2 */
void map_drawTile_trn(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget)
{
    for( int i = 0 ; i < pObj->m_header.m_nHeight ; i++)
    {
        for(int j = 0 ; j < pObj->m_header.m_nWidth ; j++)
        {
            if(pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j] != 0)
            {
                map_put(pTarget, j + posX, i + posY, 
                pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j]);
            }
        }
    }
}

/* vertical mirror */
void map_drawTile_mirrorV(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget)
{
    for( int i = 0 ; i < pObj->m_header.m_nHeight ; i++)
    {
        for(int j = 0 ; j < pObj->m_header.m_nWidth ; j++)
        {
            if(pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j] != 0)
            {
                map_put(pTarget, -j + posX, i + posY, 
                pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j]);
            }
        }
    }
}

/* height mirror */
void map_drawTile_mirrorH(_S_MAP_OBJECT *pObj, int posX, int posY, _S_MAP_OBJECT *pTarget)
{
    for( int i = 0 ; i < pObj->m_header.m_nHeight ; i++)
    {
        for(int j = 0 ; j < pObj->m_header.m_nWidth ; j++)
        {
            if(pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j] != 0)
            {
                map_put(pTarget, j + posX, -i + posY, 
                pObj->m_pBuf[(i * pObj->m_header.m_nWidth) + j]);
            }
        }
    }
}

// 0 : 실패
// 1 : 성공
/* map save */
int map_save(_S_MAP_OBJECT *pObj, char *filename)
{
    FILE* pFile = fopen(filename, "wb");

    /**/
    fprintf(pFile, "%d", pObj->m_header.m_nWidth);
    fputs(" ", pFile);
    fprintf(pFile, "%d", pObj->m_header.m_nHeight);
    fputs(" ", pFile);
    for(int i = 0 ; i <	pObj->m_header.m_nHeight * pObj->m_header.m_nWidth ; i++) 
    {
        fprintf(pFile, "%d ", pObj->m_pBuf[i]);
    }

    /*fwrite(&MapObject.m_header, sizeof(MapObject.m_header), 1, pFile);
    int nSize = MapObject.m_header.m_nHeight * MapObject.m_header.m_nWidth;
    fwrite(MapObject.m_pBuf, nSize, 1, pFile);*/
    /**/
    fclose(pFile);

    return 1;
}

// 0 : 실패
// 1 : 성공
/* map load */
int map_load(_S_MAP_OBJECT *pObj, char *filename)
{
    FILE* pFile = fopen(filename, "rb");

    /**/
    fscanf(pFile, "%d", &pObj->m_header.m_nWidth);
    fscanf(pFile, "%d", &pObj->m_header.m_nHeight);
    
    pObj->m_pBuf = malloc( pObj->m_header.m_nHeight * pObj->m_header.m_nWidth );
    for(int i = 0 ; i <	pObj->m_header.m_nHeight * pObj->m_header.m_nWidth ; i++) 
    {
        fscanf(pFile, "%d", &pObj->m_pBuf[i]);
    }

    /*fread(&MapObject.m_header, sizeof(_S_MAP_OBJECT), 1, pFile);
    if(MapObject.m_pBuf)
    {
        free(MapObject.m_pBuf);
    }
    MapObject.m_pBuf = malloc( MapObject.m_header.m_nHeight * MapObject.m_header.m_nWidth );
    int nSize = MapObject.m_header.m_nHeight * MapObject.m_header.m_nWidth;
    MapObject.m_pBuf = malloc(nSize);
    fread(MapObject.m_pBuf, nSize, 1, pFile);*/
    /**/
    fclose(pFile);

    return 1;
}

#endif