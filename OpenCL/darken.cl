__kernel void average(__global uchar4 *a, 
		      __global uchar4 *b,
		      int imgWidth, int imgHeight,
		      __local uchar4 *pixels)
{
    int g_x = get_global_id(0);
    int g_y = get_global_id(1);
    int local_size_x = get_local_size(0)+2; // dimension of array in local mem
    int local_size_y = get_local_size(1)+2; // dimension of array in local mem
    int local_x = get_local_id(0)+1; // location of pixel in local memory
    int local_y = get_local_id(1)+1; // location of pixel in local memory
  
    int2 coord;
    coord.x=g_x;
    coord.y=g_y;
    pixels[local_y*(local_size_x)+local_x]=a[coord.y*imgWidth+coord.x];
  

    if (local_x == 1)
    {
	int2 coord_left = coord;
	if (coord_left.x>0)
	    coord_left.x-=1;
	pixels[local_y*local_size_x]=a[coord_left.y*imgWidth+coord_left.x];
    }

    if (local_x == local_size_x-2)
    {
	int2 coord_right = coord;
	if (coord_right.x<imgWidth-1)
	    coord_right.x+=1;	
	pixels[local_y*(local_size_x)+local_size_x-1]=a[coord_right.y*imgWidth+coord_right.x];
    }

    if (local_y == 1)
    {
	int2 coord_bottom = coord;
	if (coord_bottom.y>0)
	    coord_bottom.y-=1;	
	pixels[local_x]=a[coord_bottom.y*imgWidth+coord_bottom.x];
    }

    if (local_y == local_size_y-2)
    {
	int2 coord_top = coord;
	if (coord_top.y<imgHeight-1)
	    coord_top.y+=1;	
	pixels[(local_size_y-1)*(local_size_x)+local_x]=a[coord_top.y*imgWidth+coord_top.x];
    }

    barrier(CLK_LOCAL_MEM_FENCE);

  
    float4 center_f = convert_float4(pixels[local_y*(local_size_x)+local_x]);
    float4 right_f = convert_float4(pixels[local_y*(local_size_x)+(local_x+1)]);
    float4 left_f = convert_float4(pixels[local_y*(local_size_x)+(local_x-1)]);
    float4 top_f = convert_float4(pixels[(local_y+1)*(local_size_x)+local_x]);
    float4 bottom_f = convert_float4(pixels[(local_y-1)*(local_size_x)+local_x]);

    float4 res = (left_f+right_f+top_f+bottom_f)*0.25;
    b[g_y*imgWidth+g_x]=convert_uchar4(res);
}

__kernel void average_NO_LOCAL(__global uchar4 *a, 
			       __global uchar4 *b,
			       int imgWidth, int imgHeight,
			       __local uchar4 *pixels)
{
    int g_x = get_global_id(0);
    int g_y = get_global_id(1);
    int2 coord;
    coord.x=g_x;
    coord.y=g_y;
    uchar4 center=a[coord.y*imgWidth+coord.x];
    float4 center_f = convert_float4(center);
    
//  float4 res = center_f;
    /* float4 res = (center_f + left_f+right_f+up_f+down_f)*0.25; */
    float4 res = (float4)(0);
    int2 coordl = coord;
    for(int x = -2; x <= 2; ++x)
    {
	/* coordl.x = clamp(coord.x + x, 0, imgWidth-1); */
	/* clamp x to 0, imgwidth-1 */
	coordl.x = coord.x + x;
	coordl.x = (coordl.x < 0 ? 0 :
		    (coordl.x > imgWidth-1 ? imgWidth-1 : coordl.x));
	
	for(int y = -2; y <= 2; ++y)
	{
	    if((0 == x && 0 == y) ||
	       (abs(x) + abs(y) > 2))
		continue;
   
	    /* coordl.y = clamp(coord.y + y, 0, imgHeight-1); */
	    /* clamp y to 0, imgheight-1 */
	    coordl.y = coord.y + y;
	    coordl.y = (coordl.y < 0 ? 0 :
			(coordl.y > imgHeight-1 ? imgHeight-1 : coordl.y));
	    
	    res += (convert_float4(a[coordl.y*imgWidth+coordl.x])) / 12;
	}
    }

    /* float4 avg = convert_float4(res); */
  
    b[g_y*imgWidth+g_x]=convert_uchar4(res);
}
