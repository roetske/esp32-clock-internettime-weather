String degreeToCompass(int angle)
 {
	//working with angles in degree
	//returns one of the 16 compass azimuth

	if (angle < 0.0) return "ERROR";
	else if (angle < 11.25) return "N";			// N
	else if (angle < 33.75) return "NNE";		// NNE
	else if (angle < 56.25) return "NE";		// NE
	else if (angle < 78.75) return "ENE";		// ENE
	else if (angle < 101.25) return "E";		// E
	else if (angle < 123.75) return "ESE";		// ESE
	else if (angle < 146.25) return "SE";		// SE
	else if (angle < 168.75) return "SSE";		// SSE
	else if (angle < 191.25) return "S";		// S
	else if (angle < 213.75) return "SSW";		// SSO
	else if (angle < 236.25) return "SW";		// SO
	else if (angle < 258.75) return "WSW";		// OSO
	else if (angle < 281.25) return "W";		// O
	else if (angle < 303.75) return "WNW";		// ONO
	else if (angle < 326.25) return "NW";		// NO
	else if (angle < 348.75) return "NNW";		// NNO
	else if (angle <= 360.0) return "N";		// N

	return "ERROR";
}
//meters per second to kilometer per hour
int mpstokmh(float mps)
{
  float kmh = mps * 3.6;
  int result = kmh + 0.5; //rounded 22.6 =>23
  return result;
}
//float to int also neg
int floattoint(float x)
{ int neg = 1;
  if (x <0)
   {neg =-1;}
  int result = x +0.5*neg;
  return result;
}