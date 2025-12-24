Mentalmap:
1. First of all we initialise window, renderer, and texture and allocate the screen pixels.
2. Then we update the screen pixels however we like, this is all been done in the cpu memory for now.
3. Now we need to update the texture which basically means we are providing the updated screen pixels to the gpu.
4. Then we clear the renderer to remove current render target ie. the backbuffer.
5. Then using render copy we swap the buffers (backbuffer <-> frontbuffer). 
6. As the GPU is updated with the swap, we finally present the frame and can set a delay for required time to 
keep that state.


So, currently the software rendering for like the cursor is done manually using a 'for' loop to loop through the 
desired pixels to assign the required pixel colour individually, this provides better control but this can also be
done using SDL magic function SDL_RenderFillRect() as well, will probably do that soon.
