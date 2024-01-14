uint WangHashRand(uint seed)
{
	seed = (seed ^ 61) ^ (seed >> 16);
	seed *= 9;
	seed = seed ^ (seed >> 4);
	seed *= 0x27d4eb2d;
	seed = seed ^ (seed >> 15);
	return seed;
}

float3 RandVec3(uint SEED,float MAX, float MIN)
{
	uint rand = WangHashRand(SEED * 1847483629);
	float3 result;
	result.x = (rand % 1024) / 1024.0f;
	rand /= 1024;
	result.y = (rand % 1024) / 1024.0f;
	rand /= 1024;
	result.z = (rand % 1024) / 1024.0f;
	
	result.x = (MAX + abs(MIN)) * result.x - abs(MIN);
	result.y = (MAX + abs(MIN)) * result.y - abs(MIN);
	result.z = (MAX + abs(MIN)) * result.z - abs(MIN);

	if(result.x <= MIN)
	{
		result.x = MIN;
	}
	if (result.y <= MIN)
	{
		result.y = MIN;
	}
	if (result.z <= MIN)
	{
		result.z = MIN;
	}
	
	return result;
}