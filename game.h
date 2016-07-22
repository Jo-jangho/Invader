#ifndef __GAME_H__
#define __GAME_H__

/* var */
int nSize = sizeof(gBullets) / sizeof(_S_BULLET_OBJECT);

/* game init */
void game_init()
{
    /* timer init */
    set_conio_terminal_mode();
    acc_tick = last_tick=0;
    system("clear");

    // game resourse roading 
    map_init(&gMap);
    map_new(&gMap, WIDTH, HEIGHT);

    map_init(&gScreenBuffer);
    map_new(&gScreenBuffer, WIDTH, HEIGHT);

    map_init(&gPlayer);
    map_load(&gPlayer, "fly.dat");

    map_init(&gMissile);
    map_load(&gMissile, "mis.dat");

    map_init(&gAlienModel);
    map_load(&gAlienModel,"alien.dat");

    // player init
    Plane_init(&gPlayerPlane, &gPlayer, WIDTH / 2, HEIGHT / 2);

    // bullet init
    for(int i = 0 ; i < nSize ; i++)
    {
        bullet_init(&gBullets[i], 0, 0, 0, &gMissile);
    }

    // alien init
    double TablePositionX[] = {1, 32, 32, 1};
    double TablePositionY[] = {1, 1, 14, 14};
    for(int i = 0 ; i < 4 ; i++)
    {
        _S_ALIEN_OBJECT *pObj = &gAlienObjects[i];
        alien_init(pObj,&gAlienModel);
        pObj->m_fXpos = TablePositionX[i];
        pObj->m_fYpos = TablePositionY[i];
        pObj->m_nFSM = 1;
        pObj->m_nStep = i + 2;
        pObj->m_pBullet = &gBullets[i];
    }
}

/* game playing */
void game_paly(double delta_tick)
{
    //  key input
    if(kbhit() != 0)
    {
        char ch = getch();
        if(ch == 'q')   //  quit
        {
            bLoop = 0;
        }
        //  player move(key input)
        Plane_Apply(&gPlayerPlane, delta_tick, ch);
    }

    //	alien Apply
    for(int i = 0 ; i < 4 ; i++) 
    {
        _S_ALIEN_OBJECT *pObj = &gAlienObjects[i];
        pObj->pfApply(pObj, delta_tick);
    }
        
    //	missile Apply
    for(int i = 0 ; i < nSize ; i++)
    {
        _S_BULLET_OBJECT *pObj = &gBullets[i];
        if(pObj->m_nFSM == 1)
        {
            bullet_apply(pObj, delta_tick);
        }
    }

    //	collied
    for(int i = 0 ; i < 10 ; i++) 
    {
        if(gBullets[i].m_nFSM != 0) 
        {
            double bullet_pos_x = gBullets[i].m_fPosX;
            double bullet_pos_y = gBullets[i].m_fPosY;

            double target_pos_x = gPlayerPlane.m_nPosX;
            double target_pos_y = gPlayerPlane.m_nPosY;

            double vx = target_pos_x - bullet_pos_x;
            double vy = target_pos_y - bullet_pos_y;
            double dist = sqrt(vx*vx+vy*vy);

            if(dist < 2) 
            {
                system("clear");
                
                setColor(YELLOW, B_MAGENTA);
                printf("\r\n========================game over========================");
                setColor(YELLOW, B_BLACK);
                printf("\r\n\r\n========================score : %d========================\r\n", score);
                setColor(RESET, RESET);
                score = 0;
                sleep(2);
                nFSM = 0;
                nStep = 0;
            }
        }
    }

    //  game score
    score_acc_tick += delta_tick;
    if(score_acc_tick > 1.0)
    {
        score++;
        score_acc_tick = 0;
    }

    //	buffer draw
    acc_tick += delta_tick;
    if(acc_tick > 0.1)
    {
        setColor(RESET, RESET);
        map_drawTile(&gMap, 0, 0, &gScreenBuffer);
        gPlayerPlane.pfDraw(&gPlayerPlane, &gScreenBuffer);
        for(int i = 0 ; i < nSize ; i++)
        {
            _S_BULLET_OBJECT *pObj = &gBullets[i];
            if(pObj->m_nFSM == 1)
            {
                bullet_draw(pObj, &gScreenBuffer);
            }
        }
        for(int i = 0 ; i < 4 ; i++) 
        {
            _S_ALIEN_OBJECT *pObj = &gAlienObjects[i];
            pObj->pfDraw(pObj, &gScreenBuffer);
        }
        gotoxy(0, 0);
        printf("-----------------------------------\r\n");
        map_dump(&gScreenBuffer);
        printf("-----------------------------------\r\n");

        puts("move : w, a, s, d\r");
        puts("quit : q\r");
        
        acc_tick = 0;
    }
}
#endif