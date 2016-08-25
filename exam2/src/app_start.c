
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);



extern int main(void);


void app_start(void)
{

    SystemInit();

    SystemCoreClockUpdate();

	main();

}




