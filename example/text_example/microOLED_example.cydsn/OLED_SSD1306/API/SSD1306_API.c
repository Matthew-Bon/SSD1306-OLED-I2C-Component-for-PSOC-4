/* ========================================
 *  Solomon Systech SSD1306 OLED Library for the Cypress PSOC 4  
 *  Written By Matthew Bon 2018
 *  Released under MIT license 
 *
 * ========================================
*/
#include <project.h>  
#include "`$INSTANCE_NAME`_SSD1306_API.h" 

struct `$INSTANCE_NAME`_OLED_Configuration;

/*
Function Name: OLED_Start() 
Arguements: none 
Returns: N/A
Description: Initializes the I2C component that the library uses to communicate 
This function should be called before using any other function in this library. 

*/
void `$INSTANCE_NAME`_OLED_Start()
{
    `$INSTANCE_NAME`_I2C_Start(); 
}
/*
Function Name: OLED_mode_select() 
Arguements: int addr, I2C address of the OLED display, 0x3C for command, 0x3D for data. 
Returns: N/A
Description: Sets the mode of the OLED display to either command or display mode 
*/

int `$INSTANCE_NAME`_OLED_mode_select(int addr)
{
    int i2c_addr = 0; 
    if (addr == display_mode)
    {
        i2c_addr = display_mode; 
        `$INSTANCE_NAME`_OLED_ADDR_Select_Write(1); //set display address line high (Display data mode) 
        return i2c_addr; 
    }
    else 
    { 
        i2c_addr = command_mode; 
        `$INSTANCE_NAME`_OLED_ADDR_Select_Write(0); //set display address line low (Display command mode) 
        return i2c_addr; 
    }
        
}

/*
Function Name: OLED_toggle_Reset() 
Arguements: none
Returns: N/A
Description: Resets the display  
*/
void `$INSTANCE_NAME`_OLED_toggle_Reset()
{
    `$INSTANCE_NAME`_OLED_Reset_Write(0); //Put display in reset
    CyDelay(500); 
    `$INSTANCE_NAME`_OLED_Reset_Write(1); //Bring display out of reset 
    return; 
}

/*
Function Name: OLED_shutdown() 
Arguements: none
Returns: N/A
Description: Puts display in reset   
*/
void `$INSTANCE_NAME`_OLED_shutdown()
{
    `$INSTANCE_NAME`_OLED_Reset_Write(0); //Puts display in reset 
    return; 
    }

/*
Function Name: OLED_wakeup() 
Arguements: none
Returns: N/A
Description: Brings display out of reset   
*/
void `$INSTANCE_NAME`_OLED_wakeup()
{
    `$INSTANCE_NAME`_OLED_Reset_Write(1); //Bring display out of reset 
    return; 
}

/*
Function Name: OLED_config() 
Arguements: OLED_Configuration structure
Returns: N/A
Description: Configures the display based upon the configuration structure passed into the function. 
*/
void `$INSTANCE_NAME`_OLED_config(struct `$INSTANCE_NAME`_OLED_Configuration *config)
{
    int addr = 0; 
    addr = `$INSTANCE_NAME`_OLED_mode_select(command_mode); 
    `$INSTANCE_NAME`_OLED_toggle_Reset();
    
    /*Begin configuration transfer*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(addr,Write,50);  
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xAE,50); // Turn Display Off
    
    /*Set Oscillator Frequency*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xD5,50); // Set OSC Freq 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x80,50); // Recommended Value
    
    /*Set Display Offset*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xD3,50); // Set Offset Commend
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->offset,50); // Offset Value
    
    /*Set Start Line*/
    if (config->start_line > 63)
    {
        /*Line parameter out of bounds, default to 0 */ 
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0, 50); 
    }
    else
    {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->start_line,50); // Set Offset Commend 
    }
    
    /*Set Segment Remap*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->segment_remap, 50); 
    
    /*Set Com Output Scan Direction*/
    if (config->scan_direction == 0xC8 || config->scan_direction == 0xC0)
    {
       `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->scan_direction,50);
    }
    else //Invalid configuration, default to 0xC0 
    {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);    
    }
    
    /*Set Com pins Config*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xDA, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->com_pins, 50); 
    
    /*Set Contrast*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x81, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(config->contrast, 50);
    
    /*Disable Entire Display on*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xA4, 50); 
    
    /*Set display to Normal or Inverse Mode*/ 
    if (config->mode == 0xA7)
    {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xA7, 50);//set display to inverse mode 
    }
    else 
    {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xA6, 50);//default to normal mode    
    }
    
    /*Enable Charge Pump*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x8D, 50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x14, 50);
    
    /*Turn display on*/ 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xAF, 50);
    
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
}

/*
Function Name: OLED_create_default_config()
Arguements: configuration structure 
Returns: N/A
Description: Populates the OLED configuration structure with the values neccesary to drive the Sparkfun Micro OLED breakout
https://www.sparkfun.com/products/13003
*/

void `$INSTANCE_NAME`_OLED_create_default_config(struct `$INSTANCE_NAME`_OLED_Configuration *defualt_config)
{
    defualt_config->offset = 0;
    defualt_config->start_line = 0; 
    defualt_config->segment_remap = 0xA1;
    defualt_config->scan_direction = 0xC8; 
    defualt_config->com_pins = 0x12; 
    defualt_config->contrast = 0x86;
    defualt_config->mode = 0xA6;
    return; 
    
}

/*
Function Name: OLED_create_custom_config()
Arguements: OLED_Configuration structure, 
            int start_line, 
            int segment_remap, 
            int scan_direction, 
            int com_pins, 
            int contrast, 
            int mode
Returns: N/A
Description: Populates configuration structure based on values passed into the function by the user. 
*/
void `$INSTANCE_NAME`_OLED_create_custom_config(struct `$INSTANCE_NAME`_OLED_Configuration *custom_config,int offset, int start_line, int segment_remap, int scan_direction, int com_pins, int contrast, int mode)
{
    custom_config->offset = offset;
    custom_config->start_line = start_line; 
    custom_config->segment_remap = segment_remap;
    custom_config->scan_direction = scan_direction; 
    custom_config->com_pins = com_pins; 
    custom_config->contrast = contrast;
    custom_config->mode = mode;
    return; 
    
}

/*
Function Name: OLED_scroll_stop() 
Arguements: none
Returns: N/A
Description: Stops the display scroll    
*/
void `$INSTANCE_NAME`_OLED_scroll_stop()
{
    `$INSTANCE_NAME`_OLED_mode_select(command_mode); //make sure the OLED is in command mode 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x2E,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50); 
    return; 
}

/*
Function Name: OLED_scroll_start() 
Arguements: none
Returns: N/A
Description: Starts display scroll   
*/
void `$INSTANCE_NAME`_OLED_scroll_start()
{
    `$INSTANCE_NAME`_OLED_mode_select(command_mode); //make sure the OLED is in command mode 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x2F,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50); 
    return; 
}

/*
Function Name: OLED_H_scroll_config()
Arguements: int direction, sets scroll direction
            int start_page, sets page to begin scrolling
            int end_page,   sets page to end scrolling
            int scroll_step, sets size of scroll set 
Returns: N/A
Description: Configures horizontal scrolling. 
*/
void `$INSTANCE_NAME`_OLED_H_scroll_config(int direction,int start_page,int end_page, int scroll_step)
{   
    if (start_page > end_page)
        {
            return; 
        }
    int scroll_command = 0; 
    if (direction == Left)
        {
            scroll_command = 0x27; // scroll direction = left 
        }
    else 
        {
            scroll_command = 0x26; // scroll direction = right 
        }
    
   `$INSTANCE_NAME`_OLED_scroll_stop(); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(scroll_command,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(start_page,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(scroll_step,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(end_page,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xFF,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50); 
    return; 
}

/*
Function Name: OLED_HV_scroll_config()
Arguements: int direction, sets scroll direction
            int start_page, sets page to begin scrolling
            int end_page,   sets page to end scrolling
            int scroll_step, sets size of scroll set 
            int vertical_offset, sets size of vertical scroll offset 
            int fixed_rows, sets rows that remained fixed during scrolling 
            int scroll_rows, sets rows to scroll 
Returns: N/A
Description: Configures horizontal and vertical scrolling. 
*/
void `$INSTANCE_NAME`_OLED_HV_scroll_config(int direction,int start_page,int end_page, int scroll_step, int vertical_offset, int fixed_rows, int scroll_rows)
{
      if (start_page > end_page)
        {
            return; 
        }
    int scroll_command = 0; 
    if (direction == Left)
        {
            scroll_command = 0x2A; // scroll direction = left 
        }
    else 
        {
            scroll_command = 0x29; // scroll direction = right 
        }
    
    `$INSTANCE_NAME`_OLED_scroll_stop(); 
    `$INSTANCE_NAME`_OLED_vertical_scroll_area(fixed_rows, scroll_rows); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(scroll_command,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(start_page,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(scroll_step,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(end_page,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(vertical_offset,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
    
}

/*
Function Name: OLED_vertical_scroll_area() 
Arguements: int fixed_rows
            int scroll_rows 
Returns: N/A
Description: Defines vertical scroll area    
*/
void `$INSTANCE_NAME`_OLED_vertical_scroll_area(int fixed_rows, int scroll_rows)
{
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xA3,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(fixed_rows,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(scroll_rows,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
}

/*
Function Name: OLED_set_addr_mode() 
Arguements: uint8 mode, Defines page, column, or row addressing modes. 
Returns: N/A
Description: Sets addressing mode of the display. The display supports page, column, or row addressing modes, however the library only supports page mode.  
*/
void `$INSTANCE_NAME`_OLED_set_addr_mode(uint8 mode)
{
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x20,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte((mode&0x0F),50); //only send lowest nibble 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
}

/*
Function Name: OLED_set_page_addr() 
Arguements: uint8 page, The display controller supports values of 0 through 7. Note the actual display in use may not support all values. 
Returns: N/A
Description: Sets the page to begin writing display data     
*/
void `$INSTANCE_NAME`_OLED_set_page_addr(uint8 page)
{
    `$INSTANCE_NAME`_OLED_mode_select(command_mode); 
    page = (page|0xB0); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(page,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
}

/*
Function Name: OLED_set_column_addr() 
Arguements: uint8 column, The display controller supports values of 0 through 127. Note the actual display in use may not support all values. 
Returns: N/A
Description: Sets the column to begin writing display data     
*/
void `$INSTANCE_NAME`_OLED_set_column_addr(uint8 column)
{
    uint8 upper_nibble = 0; 
    uint8 lower_nibble = 0; 
    upper_nibble = (0x10|(column>>4))+0x02;
	lower_nibble = 0x0F&column;
    
    `$INSTANCE_NAME`_OLED_mode_select(command_mode); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode,Write,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(upper_nibble,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(lower_nibble,50);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    return; 
}

/* Graphic Functions */

/*
Function Name: OLED_clear_display() 
Arguements: none 
Returns: N/A
Description: Wipes display by writing 0 to all pixels. 
*/
void `$INSTANCE_NAME`_OLED_clear_display()
{
    uint8_t i = 0; 
    uint8_t j = 0;
    `$INSTANCE_NAME`_OLED_mode_select(command_mode); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(command_mode, Write, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xAE,50); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50); 

	for (i=0; i<8; i++) {
        
		`$INSTANCE_NAME`_OLED_set_page_addr(i); 
		`$INSTANCE_NAME`_OLED_set_column_addr(0);
        `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
        `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
        
		for (j=0;j<0x80;j++) {
            `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
    	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0x00, 50);            
    		}
        
        `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
        
	    }
    return; 
    
} 

/*
Function Name: OLED_draw_horizontal_line() 
Arguements: uint8 x, Starting horizontal location of the line
            uint8 y, Vertical location of the line
            uint8 length, Length of the line
Returns: uint8 error_staus, returns 0 if no error. Returns error code if invalid parameters are entered. 
Description: Draws horizontal line across display from left to right. 
*/
uint8 `$INSTANCE_NAME`_OLED_draw_horizontal_line(uint8 x, uint8 y, uint8 length)
{
    uint8 error_status = 0;
    uint8 i; 
    uint8 page_start = 0;  //Determine which page of memory to draw the line on
    uint8 pixel_start = 0; //Determine which pixel in the page to draw the line on 
    
    /*bounds check */ 
    if (length > 64 || length ==0)
    {
        error_status = 1; //invalid line length 
        return error_status; 
    }
    
    if (x > 64)
    {
        error_status = 2; //line out of bounds on x access 
        return error_status; 
    }
    
    if (y > 48)
    {
        error_status = 3; //line out of bounds on y access 
        return error_status; 
    }
    
    if (length > (64 - x))
    {
        error_status = 4; //Line too long for starting location  
        return error_status; 
    }
    
    /*Determine y starting location*/ 
    if ((y == 0) || (y == 1)) 
    {
        page_start = 0; 
    }
    else 
    {
        page_start = 48/(y-1); 
    }
    
    pixel_start = (y % 8) +1; 
    
    `$INSTANCE_NAME`_OLED_set_page_addr(page_start); 
    
    /*Set x starting location*/ 
    `$INSTANCE_NAME`_OLED_set_column_addr(x); 
    
    /*Draw Line*/ 
    `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
        
		for (i=0; i<length; i++) {
            `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
    	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(pixel_start, 50);            
    		}
        
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    
    return error_status; 
}

/*
Function Name: OLED_draw_vertical_line() 
Arguements: uint8 x, Horizontal location of the line
            uint8 y, Starting vertical location of the line
            uint8 length, Length of the line
Returns: uint8 error_staus, returns 0 if no error. Returns error code if invalid parameters are entered. 
Description: Draws vertical line across display from bottom to top (page 0 to page 7). 
*/
uint8 `$INSTANCE_NAME`_OLED_draw_vertical_line(uint8 x, uint8 y, uint8 length)
{ 
    uint8 error_status = 0;
    uint16 i;
    uint8 page_start = 0; 
    uint8 pixel_start = 0; 
    uint8 page_numbers = 0; 
    uint8 pixel_end = 0; 
    
      /*bounds check */ 
    if (length > 48 || length ==0)
    {
        error_status = 1; //invalid line length 
        return error_status; 
    }
    
    if (x > 64)
    {
        error_status = 2; //line out of bounds on x access 
        return error_status; 
    }
    
    if (y > 48)
    {
        error_status = 3; //line out of bounds on y access 
        return error_status; 
    }
    
    if (length >  y)
    {
        error_status = 4; //Line too long for starting location  
        return error_status; 
    }
    
    /*Set starting X pixel*/ 
    `$INSTANCE_NAME`_OLED_set_column_addr(x); 
    
    /*Determine y starting location*/ 
    if ((y == 0) || (y == 1)) 
    {
        page_start = 0; 
    }
    else 
    {
        page_start = 48/(y-1); 
    }
    
    pixel_start = (y % 8) + 1; 
    
    page_numbers = (48-length)/8; 
    pixel_end = (48-length) % 8; 
    
    `$INSTANCE_NAME`_OLED_set_page_addr(page_start);
    
    /*Set x starting location*/ 
    `$INSTANCE_NAME`_OLED_set_column_addr(x); 
    
    /*Draw Line*/ 
    `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(pixel_start, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
        
		for (i=0; i<page_numbers; i++) {
            `$INSTANCE_NAME`_OLED_set_page_addr(page_start+i);
            `$INSTANCE_NAME`_OLED_set_column_addr(x); 
            `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
            `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
            `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
    	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xFF, 50);  
            `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
    		}
    `$INSTANCE_NAME`_OLED_set_page_addr(page_start+i+1);
    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(pixel_end, 50);  
    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);    
   
    return error_status; 
    
}

/*
Function Name: OLED_draw_rectangle() 
Arguements: uint8 x, Starting horizontal location of the line
            uint8 y, Starting vertical location of the line
            uint8 width, Width of the rectangle being drawn 
            uint8 height, Height of rectangle being drawn 
            uint8 fill,  Determines if the inside of the rectangle is filled or not. Not currently implemented. 
            uint8 length, Length of the line
Returns: uint8 error_staus, returns 0 if no error. Returns error code if invalid parameters are entered. 
Description: Draws horizontal line across display from left to right. 
*/
uint8 `$INSTANCE_NAME`_OLED_draw_rectangle(uint8 x, uint8 y, uint8 width, uint8 height, uint8 fill)
{
    uint8 error_status = 0; 
    uint8 i,j;
    uint8 page_start = 0; 
    uint8 pixel_start = 0; 
    uint8 page_numbers = 0; 
    uint8 pixel_end = 0; 
    
    /*bounds check */ 
    if (width > 64 || width ==0)
    {
        error_status = 1; //invalid rectangle width
        return error_status; 
    }
    
    if (x > 64)
    {
        error_status = 2; //rectangle start out of bounds on x access 
        return error_status; 
    }
    
    if (y > 48)
    {
        error_status = 3; //rectangle start of bounds on y access 
        return error_status; 
    }
    
    if ( height > 48 || height == 0  )
    {
        error_status = 4; //Invalid rectangle height 
        return error_status; 
    }
    
    if ( (height > (64-y)) || (width > (48-x)) )
    {
        error_status = 5; //rectangle too large for starting location 
        return error_status; 
    }
    
    
    /*Determine y starting location*/ 
    if (y == 0) 
    {
        page_start = 0; 
    }
    else 
    {
        page_start = (y-1)/8;  
    }
    
    pixel_start = (y % 8); 
    
    switch (pixel_start){
        
        case 0:
        {
            pixel_start = 0xFF; 
            break; 
        }
        
        case 1:
        {
            pixel_start = 0xFF; 
            break; 
        }
        
        case 2:
        {
            pixel_start = 0xFE; 
            break; 
        }
        
         case 3:
        {
            pixel_start = 0xFC; 
            break; 
        }
        
        case 4:
        {
            pixel_start = 0xF8; 
            break; 
        }
        
        case 5:
        {
            pixel_start = 0xF0; 
            break; 
        }
        
        case 6:
        {
            pixel_start = 0xE0; 
            break; 
        }
        
        case 7:
        {
            pixel_start = 0xC0; 
            break; 
        }
        
    }
        
    page_numbers = height/8; 
    pixel_end = height % 8; 
    
    
      switch (pixel_end){
        
        case 0:
        {
            pixel_end = 0x0; 
            break; 
        }
        
        case 1:
        {
            pixel_end = 0x01; 
            break; 
        }
        
        case 2:
        {
            pixel_end = 0x03; 
            break; 
        }
        
         case 3:
        {
            pixel_end = 0x07; 
            break; 
        }
        
        case 4:
        {
            pixel_end = 0x0F; 
            break; 
        }
        
        case 5:
        {
            pixel_end = 0x1F; 
            break; 
        }
        
        case 6:
        {
            pixel_end = 0x3F; 
            break; 
        }
        
        case 7:
        {
            pixel_end = 0x7F; 
            break; 
        }
        
    }
    
    `$INSTANCE_NAME`_OLED_set_page_addr(page_start);
    
     /*Set x starting location*/ 
    `$INSTANCE_NAME`_OLED_set_column_addr(x); 
    
    /*Draw Rectangle*/ 
    `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
        for(j=0; j<width; j++)
                {
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(pixel_start, 50);  
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
        
		for (i=0; i<page_numbers; i++) {
            `$INSTANCE_NAME`_OLED_set_page_addr(page_start+i+1);
            `$INSTANCE_NAME`_OLED_set_column_addr(x); 
            `$INSTANCE_NAME`_OLED_mode_select(display_mode);         
            for(j=0; j<width; j++)
                {
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xFF, 50);  
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
    		}
        
        `$INSTANCE_NAME`_OLED_set_page_addr(page_start+i+1);
        `$INSTANCE_NAME`_OLED_set_column_addr(x); 
        `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
        for(j=0; j<width; j++)
                {
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(pixel_end, 50);  
                    `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }   
    
    return error_status; 
}

/*
Function Name: OLED_bulk_write() 
Arguements: uint8 *data, pointer to array which contains data to be written to display
            uint8 x, horizontal size of data to be written to display    
            uint8 y, vertical size of data to be written to display, aka the number of memory pages that the data covers  
Returns: N/A
Description: Writes data from an array to display. 
*/
void `$INSTANCE_NAME`_OLED_bulk_write(uint8 *data, uint8 x, uint8 y)
{
  uint8_t i, j;
  y = y/8; 
    
	for (i=0; i<y; i++) {
		`$INSTANCE_NAME`_OLED_set_page_addr(i); 
		`$INSTANCE_NAME`_OLED_set_column_addr(0);
        `$INSTANCE_NAME`_OLED_mode_select(display_mode); 
        `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
        
		for (j=0;j<x;j++) {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0, 50);
	    `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(*data, 50);  
        data++; 
		}
        `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50); 
	}
    return; 
}

/*
Function Name: OLED_write_characters() 
Arguements: char *data, Pointer to array of characters to write to display
            uint8 row, Row/page of the display to write character string to. (Limited to 6)
            uint8 column, Column to begin writing character string, broken into divison of 8.  (Limited to 8) 
            uint8 length, Length of string to write to display (limit 8 characters, if larger truncates string to length of 8 characters) 
Returns: uint8 error_staus, returns 0 if no error. Returns error code if invalid parameters are entered. 
Description: Writes character string to display. 
*/
int `$INSTANCE_NAME`_OLED_write_characters(char *data, uint8 row, uint8 column, uint8 length) 
{
    int error_status = 0; 
    char character = 0; 
    char display_string[9] = {'\n','\n','\n','\n','\n','\n','\n','\n','\n'}; 
    uint8 i = 0; 
    uint8 column_start = 0; 
    uint8 length_check = 0; 
    
    if (length > 8)
    {
        length_check = 8; 
    }
    
    else 
    {
     
        length_check = length; 
        
    }
    
    /*string BS*/ 
    for(i = 0; i <length_check; i++)
    {
        display_string[i] = data[i]; 
        
    }
    
    i = 0; 
    
    /*Bounds Check*/ 
    if (row < 1 || row > 6) 
    {
        error_status = 1; //invalid row number 
        return error_status; 
    }
    
    if (column < 1 || column > 8) 
    {
        error_status = 2; //invalid column number 
        return error_status; 
    }
    
    /*Set starting page/row */ 
    `$INSTANCE_NAME`_OLED_set_page_addr(row); 
    
    /*Set starting column */ 
    column_start = 8 * column-1; 
    `$INSTANCE_NAME`_OLED_set_column_addr(column_start); 
    
  `$INSTANCE_NAME`_OLED_mode_select(display_mode); 

    while (display_string[i] != '\n')
    {
        char character = display_string[i]; 
        
        switch (character)
        {
          case 'A':
            {
                for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
            }
          case 'a':
            {
                for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
                
            }
            
          case 'B':
            {
                for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+8], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
                
            }
          case 'b':
            {
                for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+8], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
          case 'C':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+16], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
                
            }
          case 'c':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+16], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'D':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+24], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'd':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+24], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'E':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+32], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'e':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+32], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'F':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+40], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case 'f':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+40], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 'G':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+48], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
                
            }
          case 'g':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+48], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case 'H':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+56], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
          case 'h':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+56], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;
                
            }
            
          case 'I':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+64], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'i':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+64], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 'J':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+72], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
            
          case 'j':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+72], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;     
                
            }
            
          case 'K':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+80], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
          case 'k':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+80], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'L':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+88], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
                
            }
          case 'l':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+88], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
          case 'M':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+96], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;     
                
            }
          case 'm':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+96], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
            
          case 'N':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+104], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;      
                
            }
            
          case 'n':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+104], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;      
                
            }
            
          case 'O':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+112], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
            
          case 'o':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+112], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'P':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+120], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
          case 'p':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+120], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'Q':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+128], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case 'q':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+128], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case 'R':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+136], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case 'r':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+136], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 'S':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+144], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 's':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+144], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'T':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+152], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 't':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+152], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'U':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+160], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case 'u':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+160], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 'V':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+168], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
          case 'v':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+168], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
          case 'W':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+176], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case 'w':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+176], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case 'X':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+184], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case 'x':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+184], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'Y':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+192], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break; 
                
            }
            
          case 'y':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+192], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
                
            }
            
          case 'Z':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+200], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case 'z':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+200], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
            
         case '!':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+208], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case '.':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+216], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case ':':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+224], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '(':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+232], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case ')':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+240], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '%':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+248], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '$':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+256], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '+':
            {
              for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+264], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;    
                
            }
          case '-':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+272], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '=':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+280], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case '?':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+288], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '0':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+296], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case '1':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+304], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case '2':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+312], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
          case '3':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+320], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
                
            }
            
          case '4':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+328], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
          case '5':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+336], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '6':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+344], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '7':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+352], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
            
          case '8':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+360], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
          case '9':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+368], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;   
                
            }
                      
          case ' ':
            {
               for(uint8 j=0; j< 8; j++)
                {
                `$INSTANCE_NAME`_I2C_I2CMasterSendStart(display_mode,Write, 50); 
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(0xC0,50);
            	`$INSTANCE_NAME`_I2C_I2CMasterWriteByte(`$INSTANCE_NAME`_Oled_characters[j+376], 50);  
                `$INSTANCE_NAME`_I2C_I2CMasterSendStop(50);
                }
                break;  
                
            }
            
            
        }
        
        i++; 
        
        
    }
    
    
    
  return error_status; 
}

/* [] END OF FILE */
