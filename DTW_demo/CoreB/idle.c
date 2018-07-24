/* Simple program to maintain core B at idle to ensure that both the PLL 
   can be configured and that timers can be enabled from Core A.
*/

void main() {
    /* The following line is required to prevent a breakpoint
       being set on the asm("IDLE;"); statement
     */
    int i= 0;
    
    while(1) {
        asm("IDLE;");
    }
}
