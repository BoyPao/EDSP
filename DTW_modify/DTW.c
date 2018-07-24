#include "DSP_Lab1_exp5.h"


#include<math.h>
#include <math_const.h>

int compute_dtw(void)
{
    short i,j,k,n;
    int a,b,min=0,dis,choose;
    for(n=0;n<4;n++)
    {
    for(i=0;i<frame_count;i++)
    {
        for(j=0;j<frame_count;j++)
        {
            for(k=0;k<30;k++)
            {
            	if(n==0) a=(int)zero[i*30+k];
            	else if(n==1) a=(int)one[i*30+k];
            	else if(n==2) a=(int)two[i*30+k];
            	else a=(int)three[i*30+k];
            	
            	b=(int)test[j*30+k];
            	
                DTW[i][j]+=pow(( a-b ),2);
                
            }
        }
    }
    for(i=0;i<frame_count;i++)
    {
        for(j=0;j<frame_count;j++)
        {
            if(i==0&&j==0)
            
            	c[i][j]=DTW[i][j];

            else if(i==0&&j>0)
            
                c[i][j]=c[i][j-1]+DTW[i][j];

            else if(j==0&&i>0)
            
                c[i][j]=c[i-1][j]+DTW[i][j];

            else
            {
                min=c[i-1][j-1];
                
                if(min>c[i][j-1]) min=c[i][j-1];
                
                if(min>c[i-1][j]) min=c[i-1][j];
                
                c[i][j]=DTW[i][j]+min;
                
            }
        }
    }
    Distance4[n]=c[frame_count-1][frame_count-1]/(frame_count-1)^2;
    }
    dis=Distance4[0];
    if(dis>Distance4[1]) dis=Distance4[1];
    if(dis>Distance4[2]) dis=Distance4[2];
    if(dis>Distance4[3]) dis=Distance4[3];
    for (i=0;i<4;i++)
    {
    	 if(Distance4[i]==dis)
    	 choose=i+1;
    	 
    }
    return choose;
}
