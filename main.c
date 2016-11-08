/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
  and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_STRETCH,
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void terminate();

//Loads individual image
SDL_Surface* loadSurface(char const *path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize PNG loading 
            int imgFlags = IMG_INIT_PNG; 
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) 
            { 
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() ); success = false; 
            }
            else
            {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }

        return success;
    }

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load default surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "resources/graphics/press.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
        {
            printf( "Failed to load default image!\n" );
            success = false;
        }

        //Load up surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "resources/graphics/up.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
        {
            printf( "Failed to load up image!\n" );
            success = false;
        }

        //Load down surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "resources/graphics/down.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
        {
            printf( "Failed to load down image!\n" );
            success = false;
        }

        //Load left surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "resources/graphics/left.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
        {
            printf( "Failed to load left image!\n" );
            success = false;
        }

        //Load right surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "resources/graphics/right.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
        {
            printf( "Failed to load right image!\n" );
            success = false;
        }
        //Load stretched surface
        gKeyPressSurfaces[ KEY_PRESS_SURFACE_STRETCH ] = loadSurface( "resources/graphics/stretch.bmp" );
        if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_STRETCH] == NULL )
        {
            printf( "Failed to load right image!\n" );
            success = false;
        }   

        return success;
    }

    void terminate()
    {
        //Deallocate surfaces
        for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
        {
            SDL_FreeSurface( gKeyPressSurfaces[ i ] );
            gKeyPressSurfaces[ i ] = NULL;
        }

        //Destroy window
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;

        //Quit SDL subsystems
        SDL_Quit();
    }

    SDL_Surface* loadSurface( char const *path)
    {
        //The final, optimized image
        SDL_Surface* optimizedSurface = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load( path);
        if( loadedSurface == NULL )
        {
            printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        else
        {
            //Convert the surface to the format of the current screen
            optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0);
            if( optimizedSurface == NULL)
            {
                printf("Unable to optimize image %s! SDL ERROR %s\n", path, SDL_GetError());
            }
            //We no longer need the loaded surface, free that memory.
            SDL_FreeSurface(loadedSurface);
        }


        return optimizedSurface;
    }


    int main( int argc, char* args[] )
    {
        //Start up SDL and create window
        if( !init() )
        {
            printf( "Failed to initialize!\n" );
        }
        else
        {
            //Load media
            if( !loadMedia() )
            {
                printf( "Failed to load media!\n" );
            }
            else
            {	
                //Main loop flag
                bool quit = false;

                //Event handler
                SDL_Event e;

                //Set default current surface
                gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

                //While application is running
                while( !quit )
                {
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        //User presses a key
                        else if( e.type == SDL_KEYDOWN )
                        {
                            //Select surfaces based on key press
                            switch( e.key.keysym.sym )
                            {
                                case SDLK_UP:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                                    break;

                                case SDLK_DOWN:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                                    break;

                                case SDLK_LEFT:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                                    break;

                                case SDLK_RIGHT:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                                    break;

                                case SDLK_o:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_STRETCH];
                                    break;

                                default:
                                    gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                                    break;
                            }
                        }
                    }

                    //Apply the current image
                    SDL_Rect stretched;
                    stretched.x=0;
                    stretched.y=0;
                    stretched.w=SCREEN_WIDTH;
                    stretched.h=SCREEN_HEIGHT;
                    //Copy from arg1, rectangle, onto arg3, rectangle
                    SDL_BlitScaled(gCurrentSurface,NULL,gScreenSurface,&stretched);

                    //Update the surface
                    SDL_UpdateWindowSurface( gWindow );
                }
            }
        }

        //Free resources and close SDL
        terminate();

        return 0;
    }
