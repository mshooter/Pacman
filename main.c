///
/// @file main.c
/// @brief This is a pacman game:
/// The module draws the maze with pills and powerpills. It draws the pacman too.
/// The pacman moves (and is animated) where it should move and it teleports from one side to another.
/// It eats the (power) pills.

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
//#include <SDL2/SDL_mixer.h>

// include the map for the maze.
#include "map.h"

// the size of the block to draw
const int BLOCKSIZE=25;

// the width of the screen taking into account the maze and block
#define WIDTH COLS*BLOCKSIZE

// the height of the screen taking into account the maze and block
#define HEIGHT ROWS*BLOCKSIZE

// an enumeration for direction to move USE more enums!
enum DIRECTION{LEFT,RIGHT,UP,DOWN,NONE};


//----------------------------------------------------------------------------------------------------------------------
/// @brief function that draws the maze
/// @param[in]  ren the renderer
/// @param[in]  block the pointer of creating a SDL_rect
//----------------------------------------------------------------------------------------------------------------------
void drawMaze(SDL_Renderer* ren, SDL_Rect* block)
{
    for(int i=0; i<ROWS; ++i)
    {
        for(int j=0; j<COLS; ++j)
        {

            block->x=j*BLOCKSIZE;
            block->y=i*BLOCKSIZE;
            block->w=BLOCKSIZE;
            block->h=BLOCKSIZE;
            switch(map[i][j])
            {
              // drawing black wall
              case 0:
              {
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                break;
              }
              // drawing blue wall
              case 1:
              {
                SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
                break;
              }
              // drawing 'red' pill
              case 2:
              {
                SDL_SetRenderDrawColor(ren, 255, 160, 122, 255);
                block->x+=(BLOCKSIZE/2);
                block->y+=(BLOCKSIZE/2);
                block->w*=0.25;
                block->h*=0.25;
                break;
              }
               // drawing power pill
               case 3:
               {
                 SDL_SetRenderDrawColor(ren, 255, 160, 122, 255);
                 block->x+=(BLOCKSIZE/4);
                 block->y+=(BLOCKSIZE/4);
                 block->w*=0.5;
                 block->h*=0.5;
                 break;
               }
                //the prison bar from the ghosts
                case 4:
                {
                  SDL_SetRenderDrawColor(ren, 255, 128, 144, 255);
                  block->y+=(BLOCKSIZE/2);
                  break;
                }
            }

            SDL_RenderFillRect(ren,block);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief drawing pacman onto the screen; animation is included
/// @param[in]  ren the renderer
/// @param[in]  tex the texturer
/// @param[in]  block the pointer of creating a SDL_rect
/// @param[in]  pacman the pointer of creating a SDL_rect for texture
/// @param[in]  direction the pointer to direction for the pacman
/// @param[in]  posPacx the position x of pacman
/// @param[in]  posPacy the position of y of pacman
//----------------------------------------------------------------------------------------------------------------------
void drawPacman(SDL_Renderer* ren,SDL_Texture* tex, SDL_Rect* block, SDL_Rect* pacman, int *direction, int posPacx, int posPacy)
{
    block->x=posPacx;
    block->y=posPacy;
    block->h=BLOCKSIZE;
    block->w=BLOCKSIZE;
    // each sprite is 22 x 20 so set the width and height
    pacman->w=22;
    pacman->h=20;

    /// The following section is from:-
    /// Bilal Cheema. Game Development with SDL2 Part 4: Animating using Sprite Sheets [online].
    /// Available from: https://www.caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html
    /// [Accessed 2016]
        // total number of frames of the animation
        int totalFrames=7;

        // amount of delay in milliseconds for each frame
        int delayPerFrame=50;

        // SDL_GetTicks() method gives us the time in milliseconds
        // 'frame' will give us the index of frame we want to render
        // For example, if 'frame' is 2, it will give us the third frame
        int frame=(SDL_GetTicks()/delayPerFrame)%totalFrames;

        // The index of frmae is multiplied by the width of the frame
        // This will guve us the appropriate frame dimenstions from the sprite sheet
        pacman->y=pacman->h*frame;
   /// end of Citation
   ///
    pacman->x=(*direction)*pacman->w;
    // finally draw using the SDL_RenderCopy function
    // this takes the texture and copies the portion from the rect
    SDL_RenderCopy(ren, tex,pacman, block);
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief makes the pacman move to any direction
/// @param[in]  posPacx the pointer for position x for pacman
/// @param[in]  posPacy the pointer for position y for pacman
/// @param[in]  direction the direction of pacman
//----------------------------------------------------------------------------------------------------------------------
void movePacman(int *posPacx, int *posPacy, int direction)
{
    switch(direction)
    {
    case UP:    { (*posPacy)-=2;  break; }
    case DOWN:  { (*posPacy)+=2;  break; }
    case LEFT:  { (*posPacx)-=2;  break; }
    case RIGHT: { (*posPacx)+=2;  break; }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief it looks if there is a wall or not
/// @param[in]  direction the direction of pacman
/// @param[in]  posPacx the position x of pacman
/// @param[in]  posPacy the position y of pacman
//----------------------------------------------------------------------------------------------------------------------
int isNoWall(int direction, int posPacx, int posPacy)
{
    int space=(BLOCKSIZE-2)/2;
    switch( direction )
    {
        case UP:    { posPacy-=2;  break; }
        case DOWN:  { posPacy+=2;  break; }
        case LEFT:  { posPacx-=2;  break; }
        case RIGHT: { posPacx+=2;  break; }

    }

    //corners of pacman
    int a=(int)(round((posPacx+space)/(float)BLOCKSIZE));
    int b=(int)(round((posPacy+space)/(float)BLOCKSIZE));
    int c=(int)(round((posPacx+space)/(float)BLOCKSIZE));
    int d=(int)(round((posPacy-space)/(float)BLOCKSIZE));
    int e=(int)(round((posPacx-space)/(float)BLOCKSIZE));
    int f=(int)(round((posPacy-space)/(float)BLOCKSIZE));
    int g=(int)(round((posPacx-space)/(float)BLOCKSIZE));
    int h=(int)(round((posPacy+space)/(float)BLOCKSIZE));
    int valueRightDownCorner=map[b][a];
    int valueRightUpCorner=map[d][c];
    int valueLeftUpCorner=map[f][e];
    int valueLeftDownCorner=map[h][g];

    if(valueRightDownCorner==1 || valueRightUpCorner==1 || valueLeftUpCorner==1 || valueLeftDownCorner==1 || valueRightDownCorner==4)
        return 0;
    else
        return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief When pacman eats a pill the pill dissapears
/// @param[in]  posPacx the pointer to pacmans x position
/// @param[in]  posPacy the pointer to pacmans y position
/// @param[in]  pacmanChomp the sound of pacman when eats pill
/// @param[in]  pacmanPowerPill the sound of pacman when eats powerpill
//----------------------------------------------------------------------------------------------------------------------
void clearRedPill(int *posPacx, int *posPacy/*, Mix_Chunk* pacmanChomp, Mix_Chunk* pacmanPowerPill*/)
{
    int a=round((*posPacx)/(float)BLOCKSIZE);
    int b=round((*posPacy)/(float)BLOCKSIZE);
    //if-statement changes the value, 2(redPill), to value 0.(BlackSquare)
    if(map[b][a]==2)
    {
        map[b][a]=0;
        //Mix_PlayChannel(-1, pacmanChomp, 0); //MAC VERSION
        // sound for LINUX
        printf("\a\n");

    }
    if(map[b][a]==3)
    {
        map[b][a]=0;
        printf("\a\n");
        //Mix_PlayChannel(-1, pacmanPowerPill,0); //MAC VERSION
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief  teleports pacman from one side to the other
/// @param[in]  posPacx the pointer to pacmans x position
//----------------------------------------------------------------------------------------------------------------------
void teleportPacman(int *posPacx)
{
    if(*posPacx < 5)
        *posPacx=WIDTH-30;
    else if(*posPacx>WIDTH-30)
        *posPacx=5;
}


int main()
{
  // initialise SDL and check that it worked otherwise exit
  // see here for details http://wiki.libsdl.org/CategoryAPI
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }

  // we are now going to create an SDL window
  SDL_Window *win = 0;
  win = SDL_CreateWindow("Pacman", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (win == 0)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }

  // the renderer is the core element we need to draw, each call to SDL for drawing will need the
  // renderer pointer
  SDL_Renderer *ren = 0;
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == 0)
  {
    printf("%s\n",SDL_GetError() );
    return EXIT_FAILURE;
  }

  // SDL image is an abstraction for all images
  SDL_Surface *image;

  // we are going to use the extension SDL_image library to load a png, documentation can be found here
  // http://www.libsdl.org/projects/SDL_image/
  image=IMG_Load("pacsprite.png");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }

  // SDL texture converts the image to a texture suitable for SDL rendering  / blitting
  // once we have the texture it will be store in hardware and we don't need the image data anymore
  SDL_Texture *tex = 0;
  tex = SDL_CreateTextureFromSurface(ren, image);
  // free the image
  SDL_FreeSurface(image);


  /*MAC VERSION FOR SOUND
  /// the following section is from:-
  /// Mr. Foo, 2014. Sound Effects and Music [onine]. Second Paragraph, fourth and fifth paragraph
  /// Available from: http://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php [Accessed January 2 2017]

   // sound effects that we are going to use
   Mix_Chunk *pacmanChomp = NULL;
   Mix_Chunk *pacmanPowerPill = NULL;

   // initialise SDL_mixer
   if(Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
   {
       printf("SDL_mixer could not initialize SDL_mixer error: %s\n", Mix_GetError());
       return EXIT_FAILURE;
   }

   // Load the sound effects
   pacmanChomp = Mix_LoadWAV("Snap-SoundBible.com-405224438.wav");
   pacmanPowerPill = Mix_LoadWAV("chime_down.wav");
   if(!pacmanChomp || ! pacmanPowerPill)
   {
       printf("Failed to load pacman_chomp sound effect, SDL_mixer Error: %s\n", Mix_GetError());
       return EXIT_FAILURE;
   }
   /// end of Citation
    */

  // structure of different directions
  struct directions
  {
      int currentDirection;
      int nxtDirection;
      int tmpDirection;
  }direction;
  direction.nxtDirection=LEFT;
  direction.currentDirection=NONE;
  direction.tmpDirection=NONE;

  // pacmans position
  struct posistionPacman
  {
      int x;
      int y;
  }posPacman;
  posPacman.x=325;
  posPacman.y=426;


  int quit=0;
  // now we are going to loop forever, process the keys then draw
  while (quit !=1)
  {
    SDL_Event event;
    // grab the SDL even (this will be keys etc)
    while (SDL_PollEvent(&event))
    {
      // look for the x of the window being clicked and exit
      if (event.type == SDL_QUIT)
        quit = 1;
      // check for a key down
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
            // if we have an escape quit
            case SDLK_ESCAPE : quit=1; break;
            // moving keys

            // UP_key
            case SDLK_UP:   { direction.nxtDirection=UP;    break; }
            case SDLK_w:    { direction.nxtDirection=UP;    break; }

            // DOWN_key
            case SDLK_DOWN: { direction.nxtDirection=DOWN;  break; }
            case SDLK_s:    { direction.nxtDirection=DOWN;  break; }

            // LEFT_key
            case SDLK_LEFT: { direction.nxtDirection=LEFT;  break; }
            case SDLK_a:    { direction.nxtDirection=LEFT;  break; }

            // RIGHT_key
            case SDLK_RIGHT:    { direction.nxtDirection=RIGHT; break; }
            case SDLK_d:    { direction.nxtDirection=RIGHT; break; }

       }
    }
  }

  // now we clear the screen (will use the clear colour set previously)
  // makes the background black
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // creating a rect to draw a block
  // creating a rect for the texture
  SDL_Rect block;
  SDL_Rect pacman;

  // draws the maze
  drawMaze(ren,&block);

  // draws pacman
  drawPacman(ren, tex, &block, &pacman, &direction.tmpDirection, posPacman.x, posPacman.y);

  // makes pacman move and checks if there is a wall
  if(isNoWall(direction.nxtDirection,posPacman.x,posPacman.y))
    {
        movePacman(&posPacman.x,&posPacman.y,direction.nxtDirection);
        direction.tmpDirection=direction.currentDirection;
        direction.currentDirection=direction.nxtDirection;
    }
    else
    {
        if(isNoWall(direction.tmpDirection,posPacman.x,posPacman.y))
        {
            movePacman(&posPacman.x,&posPacman.y,direction.tmpDirection);
            direction.currentDirection=direction.tmpDirection;
        }
    }

  // clears the pills when it is eaten
  clearRedPill(&posPacman.x, &posPacman.y/*, pacmanChomp, pacmanPowerPill*/);

  // pacmans teleportation from one side to the other
  teleportPacman(&posPacman.x);

  // Up until now everything was drawn behind the scenes.
  // This will show the new, red contents of the window.
  SDL_RenderPresent(ren);
  }

 /* //Free the sound effect
  Mix_FreeChunk(pacmanPowerPill);
  pacmanPowerPill = NULL;
  Mix_FreeChunk(pacmanChomp);
  pacmanChomp = NULL;

  //Quit SDL_mixer
  Mix_Quit();

*/
  //Quit SDL
  SDL_Quit();
  return EXIT_SUCCESS;
}
