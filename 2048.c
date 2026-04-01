#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


/*==============================功能区==============================*/
//功能函数：检查结束
int EndCheck(bool flag[4][4], int number[4][4]) {
    int checkFlag = 3;
    bool emptyFlag = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (number[i][j] == 11) {
                checkFlag = 1;
            }
            if (!flag[i][j]) {
                emptyFlag = 0;
            }
        }
    }
    if (emptyFlag) {
        bool equalflag = 1;
        for (int p = 0; p < 4; p++) {
            if (number[p][0] == number[p][1] || number[p][1] == number[p][2] || number[p][2] == number[p][3]) {
                equalflag = 0;
            }
        }
        for (int q = 0; q < 4; q++) {
            if (number[0][q] == number[1][q] || number[1][q] == number[2][q] || number[2][q] == number[3][q]) {
                equalflag = 0;
            }
        }
        if (equalflag) {
            checkFlag = 2;
        }
    }

    return checkFlag;
}

//功能函数：递归计算累加值
int NumberSum(int number) {
    int sum = 0;
    if (number > 2) {
        sum = number + NumberSum(number / 2) + NumberSum(number / 2);
    }
    return sum;
}

//功能函数：渲染分数
void ScoreRender(SDL_Renderer *renderer, TTF_Font *font, int number[4][4], int *bestscore) {
    int currentscore = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (number[i][j] > 1) {
                currentscore = currentscore + NumberSum((int) pow(2, number[i][j]));
            }
        }
    }
    if (currentscore > *bestscore) {
        *bestscore = currentscore;
    }
    char current[8] = {'\0'};
    char best[8] = {'\0'};
    itoa(currentscore, current, 10);
    itoa(*bestscore, best, 10);
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface *cursurface = TTF_RenderText_Blended(font, current, color);
    SDL_Texture *curtexture = SDL_CreateTextureFromSurface(renderer, cursurface);
    SDL_Rect currect = {264 + (100 - cursurface->w) / 2, 160, cursurface->w, cursurface->h};
    SDL_RenderCopy(renderer, curtexture, NULL, &currect);
    SDL_FreeSurface(cursurface);
    SDL_DestroyTexture(curtexture);

    SDL_Surface *bestsurface = TTF_RenderText_Blended(font, best, color);
    SDL_Texture *besttexture = SDL_CreateTextureFromSurface(renderer, bestsurface);
    SDL_Rect bestrect = {398 + (100 - bestsurface->w) / 2, 160, bestsurface->w, bestsurface->h};
    SDL_RenderCopy(renderer, besttexture, NULL, &bestrect);
    SDL_FreeSurface(bestsurface);
    SDL_DestroyTexture(besttexture);
}

//功能函数：渲染首页界面
void MenuRender(SDL_Renderer *renderer, SDL_Rect rect, bool clickFlag) {
    SDL_RenderClear(renderer);
    SDL_Texture *texture = IMG_LoadTexture(renderer, "./resources/image/menuBackground.png");
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    if (clickFlag) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 50);
        SDL_RenderFillRect(renderer, &rect);
    }
}

//功能函数：渲染游戏界面
void PlayRender(SDL_Renderer *renderer, SDL_Rect rect, bool flag[4][4], int number[4][4],
           bool clickFlag, bool *mutecount, int checkFlag) {
    SDL_RenderClear(renderer);
    SDL_Texture *texture = IMG_LoadTexture(renderer, "./resources/image/playBackground.png");
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (flag[x][y]) {
                SDL_Rect backrect = {82 + 112 * x, 282 + 112 * y, 100, 100};
                SDL_Rect picrect = {100 * (number[x][y] - 1), 0, 100, 100};
                SDL_Texture *icontexture = IMG_LoadTexture(renderer, "./resources/image/numbers.png");
                SDL_RenderCopy(renderer, icontexture, &picrect, &backrect);
                SDL_DestroyTexture(icontexture);
            }
        }
    }

    if (checkFlag == 1) {
        if (*mutecount) {
            *mutecount = 0;
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                          2048);
            Mix_Music *winMusic = Mix_LoadMUS("./resources/audio/win.ogg");
            Mix_PlayMusic(winMusic, 0);
        }
        SDL_Rect winrect = {86, 421, 428, 158};
        SDL_Texture *wintexture = IMG_LoadTexture(renderer, "./resources/image/win.png");
        SDL_RenderCopy(renderer, wintexture, NULL, &winrect);
        SDL_DestroyTexture(wintexture);

    }

    if (checkFlag == 2) {
        if (*mutecount) {
            *mutecount = 0;
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                          2048);
            Mix_Music *lostMusic = Mix_LoadMUS("./resources/audio/lost.ogg");
            Mix_PlayMusic(lostMusic, 0);
        }
        SDL_Rect lostrect = {24, 421, 552, 158};
        SDL_Texture *losttexture = IMG_LoadTexture(renderer, "./resources/image/lost.png");
        SDL_RenderCopy(renderer, losttexture, NULL, &lostrect);
        SDL_DestroyTexture(losttexture);
    }

    if (clickFlag) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 50);
        SDL_RenderFillRect(renderer, &rect);
    }
}

//功能函数：随机挑选数字
void RandomNumber(bool flag[4][4], int number[4][4]) {
    bool randomflag = 0;
    while (!randomflag) {
        int i = rand() % 4;
        int j = rand() % 4;
        if (!flag[i][j]) {
            flag[i][j] = 1;
            number[i][j] = 1;
            randomflag = 1;
        }
    }
}

//功能函数：数组互相复制
void ArrayCopy(bool aflag[4][4], int anumber[4][4], bool bflag[4][4], int bnumber[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            aflag[i][j] = bflag[i][j];
            anumber[i][j] = bnumber[i][j];
        }
    }
}

//功能函数：判断两个标志数组异同
bool ArrayCheck(bool aflag[4][4], bool bflag[4][4]) {
    bool check = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (aflag[i][j] != bflag[i][j]) {
                check = 1;
                break;
            }
        }
    }
    return check;
}

//功能函数：标志和存储数组清理
void ArrayClear(bool flag[4][4], int number[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            flag[i][j] = 0;
            number[i][j] = 0;
        }
    }
}

//功能函数：行列标志和存储数组清理
void LineClear(bool flag[4], int number[4]) {
    for (int i = 0; i < 4; i++) {
        flag[i] = 0;
        number[i] = 0;
    }
}

//功能函数：合并相邻相同数字
void NumberMerge(bool flag[4], int number[4], int count) {
    for (int i = 0; i < count - 1; i++) {
        if (number[i] == number[i + 1]) {
            number[i]++;
            number[i + 1] = 0;
            flag[i + 1] = 0;
            break;
        }
    }
}

//功能函数：方向上响应函数
void DirectionUp(bool flag[4][4], int number[4][4]) {
    int count;
    int incount;
    int lineNumber[4];
    bool lineFlag[4];
    LineClear(lineFlag, lineNumber);
    for (int i = 0; i < 4; i++) {
        count = 0;
        incount = 0;
        for (int j = 0; j < 4; j++) {
            if (flag[i][j]) {
                lineFlag[count] = flag[i][j];
                lineNumber[count] = number[i][j];
                flag[i][j] = 0;
                number[i][j] = 0;
                count++;
            }
        }
        NumberMerge(lineFlag, lineNumber, count);
        for (int k = 0; k < count; k++) {
            if (lineFlag[k]) {
                flag[i][incount] = lineFlag[k];
                number[i][incount] = lineNumber[k];
                incount++;
            }
        }
        count = 0;
        incount = 0;
        LineClear(lineFlag, lineNumber);
    }
}

//功能函数：方向下响应函数
void DirectionDown(bool flag[4][4], int number[4][4]) {
    int count;
    int incount;
    int lineNumber[4];
    bool lineFlag[4];
    LineClear(lineFlag, lineNumber);
    for (int i = 0; i < 4; i++) {
        count = 0;
        incount = 0;
        for (int j = 3; j >= 0; j--) {
            if (flag[i][j]) {
                lineFlag[count] = flag[i][j];
                lineNumber[count] = number[i][j];
                flag[i][j] = 0;
                number[i][j] = 0;
                count++;
            }
        }
        NumberMerge(lineFlag, lineNumber, count);
        for (int k = 0; k < count; k++) {
            if (lineFlag[k]) {
                flag[i][3 - incount] = lineFlag[k];
                number[i][3 - incount] = lineNumber[k];
                incount++;
            }
        }
        count = 0;
        incount = 0;
        LineClear(lineFlag, lineNumber);
    }
}

//功能函数：方向左响应函数
void DirectionLeft(bool flag[4][4], int number[4][4]) {
    int count;
    int incount;
    int lineNumber[4];
    bool lineFlag[4];
    LineClear(lineFlag, lineNumber);
    for (int i = 0; i < 4; i++) {
        count = 0;
        incount = 0;
        for (int j = 0; j < 4; j++) {
            if (flag[j][i]) {
                lineFlag[count] = flag[j][i];
                lineNumber[count] = number[j][i];
                flag[j][i] = 0;
                number[j][i] = 0;
                count++;
            }
        }
        NumberMerge(lineFlag, lineNumber, count);
        for (int k = 0; k < count; k++) {
            if (lineFlag[k]) {
                flag[incount][i] = lineFlag[k];
                number[incount][i] = lineNumber[k];
                incount++;
            }
        }
        count = 0;
        incount = 0;
        LineClear(lineFlag, lineNumber);
    }
}

//功能函数：方向右响应函数
void DirectionRight(bool flag[4][4], int number[4][4]) {
    int count;
    int incount;
    int lineNumber[4];
    bool lineFlag[4];
    LineClear(lineFlag, lineNumber);
    for (int i = 0; i < 4; i++) {
        count = 0;
        incount = 0;
        for (int j = 3; j >= 0; j--) {
            if (flag[j][i]) {
                lineFlag[count] = flag[j][i];
                lineNumber[count] = number[j][i];
                flag[j][i] = 0;
                number[j][i] = 0;
                count++;
            }
        }
        NumberMerge(lineFlag, lineNumber, count);
        for (int k = 0; k < count; k++) {
            if (lineFlag[k]) {
                flag[3 - incount][i] = lineFlag[k];
                number[3 - incount][i] = lineNumber[k];
                incount++;
            }
        }
        count = 0;
        incount = 0;
        LineClear(lineFlag, lineNumber);
    }
}
/*============================================================*/


/*==============================主体区==============================*/
int main(int argc, char *argv[]) {

    //初始化SDL2
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    //创建窗口
    SDL_Window *gameWindows = SDL_CreateWindow("2048",
                                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                               600, 800,
                                               SDL_WINDOW_SHOWN);
    //创建渲染器
    SDL_Renderer *gameRenderer = SDL_CreateRenderer(gameWindows, -1, SDL_RENDERER_ACCELERATED);

    //定义首页按钮响应区坐标
    SDL_Rect startRect = {150, 520, 300, 60};

    //生成背景
    MenuRender(gameRenderer, startRect, 0);
    SDL_RenderPresent(gameRenderer);

    //生成背景音乐
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    Mix_Music *backgroundMusic = Mix_LoadMUS("./resources/audio/backgroundmusic.ogg");
    Mix_PlayMusic(backgroundMusic, -1);

    //生成字体
    TTF_Font *font = TTF_OpenFont("./resources/font/minecraft.ttf", 20);

    //生成退出事件的条件
    int menuQuit = 0;

    //定义标签，判断退出时是否启动游戏界面
    bool enterflag = 0;

    //定义事件
    SDL_Event menuEvent;

    //首页事件开始
    while (!menuQuit) {
        while (SDL_PollEvent(&menuEvent)) {
            switch (menuEvent.type) {
                case SDL_QUIT:
                    menuQuit = 1;
                    break;
                case SDL_MOUSEMOTION:
                    if (menuEvent.motion.x >= 150 && menuEvent.motion.x <= 450 &&
                        menuEvent.motion.y >= 520 && menuEvent.motion.y <= 580) {
                        MenuRender(gameRenderer, startRect, 1);
                        SDL_RenderPresent(gameRenderer);
                    } else {
                        MenuRender(gameRenderer, startRect, 0);
                        SDL_RenderPresent(gameRenderer);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (menuEvent.motion.x >= 150 && menuEvent.motion.x <= 450 &&
                        menuEvent.motion.y >= 520 && menuEvent.motion.y <= 580) {
                        Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
                        Mix_Chunk *mouseClick = Mix_LoadWAV("./resources/audio/click.ogg");
                        Mix_PlayChannel(-1, mouseClick, 0);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (menuEvent.motion.x >= 150 && menuEvent.motion.x <= 450 &&
                        menuEvent.motion.y >= 520 && menuEvent.motion.y <= 580) {
                        Mix_FreeMusic(backgroundMusic);
                        enterflag = 1;
                        menuQuit = 1;
                    }
                    break;
            }
        }
    }

    if (enterflag) {

        //生成最高分数
        int best = 0;
        bool mutecount = 1;

        //生成标志和存值数组，并赋初始值为零
        int number[4][4];
        bool flag[4][4];
        ArrayClear(flag, number);

        //生成备份标志和存值数组，并赋初始值为零，用以撤销至上一步
        int backNumber[4][4];
        bool backFlag[4][4];
        ArrayClear(backFlag, backNumber);

        //定义游戏按钮响应区坐标
        SDL_Rect undoRect = {259, 60, 110, 40};
        SDL_Rect restartRect = {393, 60, 110, 40};

        //生成两个数字图标
        RandomNumber(flag, number);
        RandomNumber(flag, number);

        //生成游戏背景
        PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount, EndCheck(flag, number));
        ScoreRender(gameRenderer, font, number, &best);
        SDL_RenderPresent(gameRenderer);

        //生成游戏事件
        int playQuit = 0;
        SDL_Event playEvent;
        while (!playQuit) {
            while (SDL_PollEvent(&playEvent)) {
                switch (playEvent.type) {
                    case SDL_QUIT:
                        playQuit = 1;
                        break;
                    case SDL_MOUSEMOTION:
                        if (playEvent.motion.x >= 259 && playEvent.motion.x <= 369 &&
                            playEvent.motion.y >= 60 && playEvent.motion.y <= 100) {
                            PlayRender(gameRenderer, undoRect, flag, number, 1, &mutecount, EndCheck(flag, number));
                            ScoreRender(gameRenderer, font, number, &best);
                            SDL_RenderPresent(gameRenderer);
                        } else if (playEvent.motion.x >= 393 && playEvent.motion.x <= 503 &&
                                   playEvent.motion.y >= 60 && playEvent.motion.y <= 100) {
                            PlayRender(gameRenderer, restartRect, flag, number, 1, &mutecount, EndCheck(flag, number));
                            ScoreRender(gameRenderer, font, number, &best);
                            SDL_RenderPresent(gameRenderer);
                        } else {
                            PlayRender(gameRenderer, undoRect, flag, number, 0, &mutecount, EndCheck(flag, number));
                            ScoreRender(gameRenderer, font, number, &best);
                            SDL_RenderPresent(gameRenderer);
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (playEvent.motion.x >= 259 && playEvent.motion.x <= 369 &&
                            playEvent.motion.y >= 60 && playEvent.motion.y <= 100 ||
                            playEvent.motion.x >= 393 && playEvent.motion.x <= 503 &&
                            playEvent.motion.y >= 60 && playEvent.motion.y <= 100) {
                            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
                            Mix_Chunk *mouseClick = Mix_LoadWAV("./resources/audio/click.ogg");
                            Mix_PlayChannel(-1, mouseClick, 0);
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
                        if (playEvent.motion.x >= 259 && playEvent.motion.x <= 369 &&
                            playEvent.motion.y >= 60 && playEvent.motion.y <= 100) {
                            ArrayCopy(flag, number, backFlag, backNumber);
                            PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount, EndCheck(flag, number));
                            ScoreRender(gameRenderer, font, number, &best);
                            SDL_RenderPresent(gameRenderer);
                        }
                        if (playEvent.motion.x >= 393 && playEvent.motion.x <= 503 &&
                            playEvent.motion.y >= 60 && playEvent.motion.y <= 100) {
                            mutecount = 1;
                            ArrayClear(flag, number);
                            ArrayClear(backFlag, backNumber);
                            RandomNumber(flag, number);
                            RandomNumber(flag, number);
                            PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount, EndCheck(flag, number));
                            ScoreRender(gameRenderer, font, number, &best);
                            SDL_RenderPresent(gameRenderer);
                        }
                        break;
                    case SDL_KEYDOWN:
                        switch (playEvent.key.keysym.sym) {
                            case SDLK_DOWN:
                                ArrayCopy(backFlag, backNumber, flag, number);
                                DirectionDown(flag, number);
                                if (ArrayCheck(backFlag, flag)) {
                                    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                                                  2048);
                                    Mix_Chunk *directionDown = Mix_LoadWAV("./resources/audio/slide.ogg");
                                    Mix_PlayChannel(-1, directionDown, 0);
                                    RandomNumber(flag, number);
                                }
                                PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount,
                                           EndCheck(flag, number));
                                ScoreRender(gameRenderer, font, number, &best);
                                SDL_RenderPresent(gameRenderer);
                                break;
                            case SDLK_UP:
                                ArrayCopy(backFlag, backNumber, flag, number);
                                DirectionUp(flag, number);
                                if (ArrayCheck(backFlag, flag)) {
                                    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                                                  2048);
                                    Mix_Chunk *directionUp = Mix_LoadWAV("./resources/audio/slide.ogg");
                                    Mix_PlayChannel(-1, directionUp, 0);
                                    RandomNumber(flag, number);
                                }
                                PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount,
                                           EndCheck(flag, number));
                                ScoreRender(gameRenderer, font, number, &best);
                                SDL_RenderPresent(gameRenderer);
                                break;
                            case SDLK_LEFT:
                                ArrayCopy(backFlag, backNumber, flag, number);
                                DirectionLeft(flag, number);
                                if (ArrayCheck(backFlag, flag)) {
                                    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                                                  2048);
                                    Mix_Chunk *directionLeft = Mix_LoadWAV("./resources/audio/slide.ogg");
                                    Mix_PlayChannel(-1, directionLeft, 0);
                                    RandomNumber(flag, number);
                                }
                                PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount,
                                           EndCheck(flag, number));
                                ScoreRender(gameRenderer, font, number, &best);
                                SDL_RenderPresent(gameRenderer);
                                break;
                            case SDLK_RIGHT:
                                ArrayCopy(backFlag, backNumber, flag, number);
                                DirectionRight(flag, number);
                                if (ArrayCheck(backFlag, flag)) {
                                    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                                                  2048);
                                    Mix_Chunk *directionRight = Mix_LoadWAV("./resources/audio/slide.ogg");
                                    Mix_PlayChannel(-1, directionRight, 0);
                                    RandomNumber(flag, number);
                                }
                                PlayRender(gameRenderer, restartRect, flag, number, 0, &mutecount,
                                           EndCheck(flag, number));
                                ScoreRender(gameRenderer, font, number, &best);
                                SDL_RenderPresent(gameRenderer);
                                break;
                        }
                }
            }
        }
    }

    //销毁内存
    TTF_CloseFont(font);
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindows);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
/*============================================================*/