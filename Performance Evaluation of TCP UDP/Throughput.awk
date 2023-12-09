BEGIN{
stime = 0
ftime = 0
flag = 0
fsize = 0
throughput = 0
latency = 0
pcktid=0
x=0
ln=0
}
{
if($1 == "r" && $4==0 && $3==2 && $5=="ack")
{
    
        printf("Line %d: %s\n", NR, $0)
        if ($11 > pcktid) {
            pcktid = $11
        }
    
}
}	
END{
printf("\nNo of succ ack : %d",pcktid)
}

