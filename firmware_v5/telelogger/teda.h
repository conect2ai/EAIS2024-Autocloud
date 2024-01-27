#include <math.h>

#define WINDOW_THRESHOLD 3
#define M_PI  3.14159265358979323846  /* pi */


class TEDA
{
private:
    int window_threshold = WINDOW_THRESHOLD;
    float k = 1;
    float m;
    float variance = 0;
    float eccentricity = 0;
    float mean = 0;
    int window_count = 0;
    int tempo = 0;
    float norm_eccentricity, outlier_threshold, last_value;

public:
    TEDA(float threshold){
      m = threshold;
    }

    virtual void resetWindow(float x)
    {
        k = 1;
        variance = 0;
        mean = 0;
        window_count = 0;
        last_value = x;
    }
    virtual float calcMean(float x)
    {
        float new_mean = (((k - 1) / k) * mean) + ((1 / k) * x);
        //Serial.print("new_mean = ");
        //Serial.println(new_mean);
        return new_mean;
    }
    virtual float calcVariance(float x)
    {
        float distance_squared = ((x - mean) * (x - mean));
        float new_var = (((k - 1) / k) * variance) + (distance_squared * (1 / (k - 1)));
        //Serial.print("new_var = ");
        //Serial.println(new_var);
        return new_var;
    }
    virtual float calcEccentricity(float x)
    {
        float new_ecc;
        float mean2 = (mean - x) * (mean - x);
        if (mean2 == 0)
        {
            new_ecc = 0;
        }
        else
        {
            new_ecc = (1 / k) + ((mean2) / (k * variance));
        }
        //Serial.print("new_ecc = ");
        //Serial.println(new_ecc);
        return new_ecc;
    }
    virtual int run(float x)
    {

        //int n = 1.5;
        tempo = tempo + 1;

        if (k == 1)
        {
            // Serial.println(k);
            mean = x;
            variance = 0;
            k = k + 1;
            last_value = x;
            if (tempo == 1)
                return 1;
            return 0;
        }
        else if (x == last_value && variance == 0)
        {
            mean = calcMean(x);
            variance = calcVariance(x);

            k = k + 1;
            last_value = x;
            return 0;
        }
        else

        {
            mean = calcMean(x);
            variance = calcVariance(x);
            eccentricity = calcEccentricity(x);
            norm_eccentricity = eccentricity / 2;

            outlier_threshold = ((m * m) + 1) / (2 * k);

            if (norm_eccentricity > outlier_threshold)
            {
                k = k + 1;
                last_value = x;
                return 1;
            }
            else
            {
                k = k + 1;
                last_value = x;
                return 0;
            }
        }
    };
};
