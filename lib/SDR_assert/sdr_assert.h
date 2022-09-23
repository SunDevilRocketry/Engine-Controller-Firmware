/*******************************************************************************
*
* FILE: 
* 		sdr_assert.h
*
* DESCRIPTION: 
* 		Contains assert functions to log asserts in embedded memory or display 
*       firmware status on an led 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Public  Prototypes
------------------------------------------------------------------------------*/


/* Creates a new log file to record assert messages */
void sdr_sd_assert_init
    (	
	void
    );

/* Logs an assert message to the SD card when the expression within the assert 
message equates to true */
void sdr_sd_assert
    (	
	bool  expression, 
    char* log_message
    );

/* Lights up the onboard LED with a specified color when the expression within 
   the assert equates to true */
void sdr_led_assert
    (	
	bool    expression, 
    uint8_t led_color 
    );


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/
