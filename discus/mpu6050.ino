#include "mpu6050.h"
#include "wifi.h"

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
extern uint64_t time_start_t;
void mpu_init()
{

      //Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    //Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  //Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  //Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    //Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    //Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    //Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    //Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  //Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    //Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    //Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    //Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    //Serial.println("+- 2000 deg/s");
    break;
  }
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  //Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    //Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    //Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    //Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    //Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    //Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    //Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    //Serial.println("5 Hz");
    break;
  }

  //Serial.println("");
  delay(100);
}


void mpu_get_event()
{

  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  //Serial.print("Acceleration X: ");
  //Serial.print(a.acceleration.x);
  //Serial.print(", Y: ");
  //Serial.print(a.acceleration.y);
  //Serial.print(", Z: ");
  //Serial.print(a.acceleration.z);
  //Serial.println(" m/s^2");

  //Serial.print("Rotation X: ");
  //Serial.print(g.gyro.x);
  //Serial.print(", Y: ");
  //Serial.print(g.gyro.y);
  //Serial.print(", Z: ");
  //Serial.print(g.gyro.z);
  //Serial.println(" rad/s");

  //Time (s),Gyroscope X (deg/s),Gyroscope Y (deg/s),Gyroscope Z (deg/s),Accelerometer X (g),Accelerometer Y (g),Accelerometer Z (g)
  float timeR =( float(millis()) - float(time_start_t)) / 1000.0F;
  //Serial.println(time_start_t);
  NVS_Save_DATA(timeR ,g.gyro.x*57.2958,g.gyro.y*57.2958,g.gyro.z*57.2958,a.acceleration.x, a.acceleration.y,a.acceleration.z);

  
}

uint8_t Get_NVS_Data(String file_data)
{

	//Serial.println("[SPIFSS]start extracting data");
	//Serial.println(file_data);
	char str[strlen(file_data.c_str())] = ""; // initializing array of characters with the size of incoming string
	strcpy(str, file_data.c_str());			  // copying the content of the incoming string into the initialized char array
	int temp_index = 0;
	char *pch;
	pch = strtok(str, " "); // splitting the char array depending on finding a space character " "
	String temp = "";
	while (pch != NULL)
	{
		temp_index++;
		switch (temp_index)
		{
		case 1:
			temp += String(pch)+" ";
			break;
		case 2:
			temp += String(pch)+" ";

			break;
		case 3:
			temp += String(pch)+" ";

			break;

		case 4:
			temp += String(pch)+" ";
			break;

		case 5:
			temp += String(pch)+" ";
			break;

		case 6:
			temp += String(pch)+" ";
			break;
		case 7:
			temp += String(pch)+" ";
			break;
		default:
			break;
		}
		pch = strtok(NULL, " ");
	}
  send(temp);
}

//  //Time (s),Gyroscope X (deg/s),Gyroscope Y (deg/s),Gyroscope Z (deg/s),Accelerometer X (g),Accelerometer Y (g),Accelerometer Z (g)

void NVS_Save_DATA(float time_t , double Gyroscope_X,double Gyroscope_Y,double Gyroscope_Z, double Accelerometer_X, double Accelerometer_Y, double Accelerometer_Z)
{
	// TODO Check if the data was avalid from a to z  , numbers
	// saved_index++;
	//Serial.println("start saving data");
	String temp = ""; // for concat the name with the index

	temp = String(time_t,5) + " ";
	temp += String(Gyroscope_X,3) + " ";
  temp += String(Gyroscope_Y,3) + " ";
  temp += String(Gyroscope_Z,3) + " ";
  temp += String(Accelerometer_X/9.8,3) + " ";
  temp += String(Accelerometer_Y/9.8,3) + " ";
  temp += String(Accelerometer_Z/9.8,3) + "\n";
	appendFile(SPIFFS, "/saved_data.txt", temp.c_str());
	//Serial.println("done saving data");
	// doc.clear();

}
