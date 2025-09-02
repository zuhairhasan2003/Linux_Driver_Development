How to test this: 

Step 1 : Open Terminal in this directory
Step 2 : Follow the commands:

    make
    sudo insmod driver.ko
    sudo mknod /dev/buttonDriver c <major number> 0
    gcc ./userApp.c
    ./a.out

Step 3 : Now press button connected to the raspberry pi


Explination:

The driver lets user programs register with it to receive signals. When the button is pressed and an interrupt occurs, the driver sends a signal to the registered program.