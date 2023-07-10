# Result of household_power_consumption.csv

## Column information

| Name                  | Type    | #DV   |
| :----                 | :----   | :---- |
| Date                  | Literal | 1442  |
| Time                  | Literal | 1440  |
| Global_active_power   | Float   | 4187  |
| Global_reactive_power | Float   | 533   |
| Voltage               | Float   | 2838  |
| Global_intensity      | Float   | 222   |
| Sub_metering_1        | Float   | 89    |
| Sub_metering_2        | Float   | 82    |
| Sub_metering_3        | Float   | 33    |


## Summary

### Use naive method

| Column Set                                  | Bit Width | #DV         | Allocated Buckets | Average Q-Error | Maximum Q-Error | Minimum Q-Error | Invalid Results |
| :----                                       | :----     | :----       | :----             | :----           | :----           | :----           | :----           |
| Global_active_power * Global_reactive_power | 8         | 4187 * 533  | 32 * 8            | 15.871874       | 270.941162      | 1.220424        | 0               |
| Global_active_power * Global_reactive_power | 12        | 4187 * 533  | 128 * 32          | 3.462181        | 37.185596       | 1.075858        | 0               |
| Global_active_power * Global_reactive_power | 16        | 4187 * 533  | 512 * 128         | 1.395577        | 5.675900        | 1.000000        | 0               |
| Global_active_power * Voltage               | 8         | 4187 * 2838 | 16 * 16           | 6.546440        | 55.485840       | 1.112351        | 0               |
| Global_active_power * Voltage               | 12        | 4187 * 2838 | 64 * 64           | 2.517533        | 21.299757       | 1.029228        | 0               |
| Global_active_power * Voltage               | 16        | 4187 * 2838 | 256 * 256         | 1.317489        | 4.853996        | 1.015872        | 0               |
| Global_active_power * Global_intensity      | 8         | 4187 * 222  | 64 * 4            | 1583.687866     | 87045.554688    | 0.000000        | 24              |
| Global_active_power * Global_intensity      | 12        | 4187 * 222  | 256 * 16          | 647.937134      | 37949.109375    | 0.000000        | 24              |
| Global_active_power * Global_intensity      | 16        | 4187 * 222  | 1024 * 64         | 1.544022        | 5.911186        | 0.000000        | 24              |
| Global_active_power * Sub_metering_1        | 8         | 4187 * 89   | 128 * 2           | 1.446010        | 5.886701        | 1.037999        | 0               |
| Global_active_power * Sub_metering_1        | 12        | 4187 * 89   | 512 * 8           | 1.151104        | 2.266658        | 1.000702        | 0               |
| Global_active_power * Sub_metering_1        | 16        | 4187 * 89   | 2048 * 32         | 1.066041        | 1.202293        | 1.000000        | 0               |
| Global_active_power * Sub_metering_2        | 8         | 4187 * 82   | 128 * 2           | 2.491206        | 21.168936       | 1.028441        | 0               |
| Global_active_power * Sub_metering_2        | 12        | 4187 * 82   | 512 * 8           | 1.935878        | 11.481524       | 1.006434        | 0               |
| Global_active_power * Sub_metering_2        | 16        | 4187 * 82   | 2048 * 32         | 1.777185        | 10.931447       | 1.000000        | 0               |
| Global_active_power * Sub_metering_3        | 8         | 4187 * 33   | 128 * 2           | 2.162548        | 6.948209        | 1.004861        | 3               |
| Global_active_power * Sub_metering_3        | 12        | 4187 * 33   | 512 * 8           | 1.742027        | 3.735195        | 1.003664        | 3               |
| Global_active_power * Sub_metering_3        | 16        | 4187 * 33   | 2048 * 32         | 1.550591        | 3.252306        | 1.000000        | 3               |
| Global_reactive_power * Voltage             | 8         | 533 * 2838  | 8 * 32            | 5.934519        | 58.932693       | 1.093197        | 0               |
| Global_reactive_power * Voltage             | 12        | 533 * 2838  | 32 * 128          | 1.845386        | 11.222070       | 1.009901        | 0               |
| Global_reactive_power * Voltage             | 16        | 533 * 2838  | 128 * 512         | 1.236164        | 6.113461        | 1.000289        | 0               |
| Global_reactive_power * Global_intensity    | 8         | 533 * 222   | 32 * 8            | 7.076643        | 67.550873       | 1.093025        | 0               |
| Global_reactive_power * Global_intensity    | 12        | 533 * 222   | 128 * 32          | 2.041884        | 13.502739       | 1.019138        | 0               |
| Global_reactive_power * Global_intensity    | 16        | 533 * 222   | 512 * 128         | 1.151392        | 2.914342        | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_1      | 8         | 533 * 89    | 32 * 8            | 1.659593        | 6.497054        | 1.020708        | 0               |
| Global_reactive_power * Sub_metering_1      | 12        | 533 * 89    | 128 * 32          | 1.161160        | 2.570947        | 1.001582        | 0               |
| Global_reactive_power * Sub_metering_1      | 16        | 533 * 89    | 512 * 128         | 1.004395        | 1.091058        | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_2      | 8         | 533 * 82    | 32 * 8            | 3.266850        | 47.960438       | 1.006519        | 0               |
| Global_reactive_power * Sub_metering_2      | 12        | 533 * 82    | 128 * 32          | 2.182979        | 34.029018       | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_2      | 16        | 533 * 82    | 512 * 128         | 1.013139        | 1.475081        | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_3      | 8         | 533 * 33    | 64 * 4            | 3.056562        | 58.837917       | 1.008497        | 0               |
| Global_reactive_power * Sub_metering_3      | 12        | 533 * 33    | 256 * 16          | 1.553450        | 4.085263        | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_3      | 16        | 533 * 33    | 1024 * 64         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Voltage * Global_intensity                  | 8         | 2838 * 222  | 64 * 4            | 9.114873        | 69.216339       | 1.108117        | 0               |
| Voltage * Global_intensity                  | 12        | 2838 * 222  | 256 * 16          | 2.940186        | 32.469807       | 1.047327        | 0               |
| Voltage * Global_intensity                  | 16        | 2838 * 222  | 1024 * 64         | 1.444898        | 3.828101        | 1.000000        | 0               |
| Voltage * Sub_metering_1                    | 8         | 2838 * 89   | 64 * 4            | 1.540462        | 9.395551        | 1.000177        | 0               |
| Voltage * Sub_metering_1                    | 12        | 2838 * 89   | 256 * 16          | 1.159124        | 2.371428        | 1.000177        | 0               |
| Voltage * Sub_metering_1                    | 16        | 2838 * 89   | 1024 * 64         | 1.054693        | 1.229215        | 1.000000        | 0               |
| Voltage * Sub_metering_2                    | 8         | 2838 * 82   | 128 * 2           | 1.843742        | 12.233607       | 1.044871        | 0               |
| Voltage * Sub_metering_2                    | 12        | 2838 * 82   | 512 * 8           | 1.673633        | 11.727378       | 1.013028        | 0               |
| Voltage * Sub_metering_2                    | 16        | 2838 * 82   | 2048 * 32         | 1.629313        | 11.672724       | 1.000000        | 0               |
| Voltage * Sub_metering_3                    | 8         | 2838 * 33   | 128 * 2           | 2.273063        | 36.427177       | 1.018855        | 0               |
| Voltage * Sub_metering_3                    | 12        | 2838 * 33   | 512 * 8           | 1.854122        | 35.646809       | 1.000656        | 0               |
| Voltage * Sub_metering_3                    | 16        | 2838 * 33   | 2048 * 32         | 1.264967        | 2.692981        | 1.000000        | 0               |
| Global_intensity * Sub_metering_1           | 8         | 222 * 89    | 32 * 8            | 1.923805        | 30.799824       | 1.020837        | 0               |
| Global_intensity * Sub_metering_1           | 12        | 222 * 89    | 128 * 32          | 1.147259        | 1.981946        | 1.000000        | 0               |
| Global_intensity * Sub_metering_1           | 16        | 222 * 89    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Global_intensity * Sub_metering_2           | 8         | 222 * 82    | 32 * 8            | 4.536639        | 157.674362      | 1.039185        | 0               |
| Global_intensity * Sub_metering_2           | 12        | 222 * 82    | 128 * 32          | 2.541106        | 69.158585       | 1.000000        | 0               |
| Global_intensity * Sub_metering_2           | 16        | 222 * 82    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Global_intensity * Sub_metering_3           | 8         | 222 * 33    | 32 * 8            | 2.788792        | 21.119431       | 1.035486        | 3               |
| Global_intensity * Sub_metering_3           | 12        | 222 * 33    | 128 * 32          | 1.595615        | 5.219274        | 1.000000        | 3               |
| Global_intensity * Sub_metering_3           | 16        | 222 * 33    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 3               |
| Sub_metering_1 * Sub_metering_2             | 8         | 89 * 82     | 16 * 16           | 2.228698        | 28.461384       | 1.016422        | 0               |
| Sub_metering_1 * Sub_metering_2             | 12        | 89 * 82     | 64 * 64           | 1.888211        | 25.982573       | 1.000000        | 0               |
| Sub_metering_1 * Sub_metering_2             | 16        | 89 * 82     | 256 * 256         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Sub_metering_1 * Sub_metering_3             | 8         | 89 * 33     | 32 * 8            | 1.591702        | 4.590410        | 1.004502        | 0               |
| Sub_metering_1 * Sub_metering_3             | 12        | 89 * 33     | 128 * 32          | 1.323815        | 2.806143        | 1.000000        | 0               |
| Sub_metering_1 * Sub_metering_3             | 16        | 89 * 33     | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Sub_metering_2 * Sub_metering_3             | 8         | 82 * 33     | 32 * 8            | 2.149617        | 13.042742       | 1.004377        | 0               |
| Sub_metering_2 * Sub_metering_3             | 12        | 82 * 33     | 128 * 32          | 1.406343        | 2.933782        | 1.000000        | 0               |
| Sub_metering_2 * Sub_metering_3             | 16        | 82 * 33     | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |

### Use uniform assumption

| Column Set                                  | Bit Width | #DV         | Allocated Buckets | Average Q-Error | Maximum Q-Error | Minimum Q-Error | Invalid Results |
| :----                                       | :----     | :----       | :----             | :----           | :----           | :----           | :----           |
| Global_active_power * Global_reactive_power | 8         | 4187 * 533  | 32 * 8            | 2.223831        | 24.061169       | 1.001121        | 0               |
| Global_active_power * Global_reactive_power | 12        | 4187 * 533  | 128 * 32          | 1.477139        | 9.916739        | 1.000426        | 0               |
| Global_active_power * Global_reactive_power | 16        | 4187 * 533  | 512 * 128         | 1.137388        | 3.564570        | 1.000006        | 0               |
| Global_active_power * Voltage               | 8         | 4187 * 2838 | 16 * 16           | 1.286847        | 2.412543        | 1.001477        | 0               |
| Global_active_power * Voltage               | 12        | 4187 * 2838 | 64 * 64           | 1.044415        | 1.605841        | 1.000035        | 0               |
| Global_active_power * Voltage               | 16        | 4187 * 2838 | 256 * 256         | 1.011694        | 1.193112        | 1.000069        | 0               |
| Global_active_power * Global_intensity      | 8         | 4187 * 222  | 64 * 4            | 112.496223      | 6844.777832     | 0.000000        | 24              |
| Global_active_power * Global_intensity      | 12        | 4187 * 222  | 256 * 16          | 27.340256       | 1605.444458     | 0.000000        | 24              |
| Global_active_power * Global_intensity      | 16        | 4187 * 222  | 1024 * 64         | 1.177969        | 2.980283        | 0.000000        | 24              |
| Global_active_power * Sub_metering_1        | 8         | 4187 * 89   | 128 * 2           | 37.887852       | 51.307739       | 1.142402        | 0               |
| Global_active_power * Sub_metering_1        | 12        | 4187 * 89   | 512 * 8           | 9.569093        | 11.469337       | 1.000387        | 0               |
| Global_active_power * Sub_metering_1        | 16        | 4187 * 89   | 2048 * 32         | 2.445135        | 2.772955        | 1.000015        | 0               |
| Global_active_power * Sub_metering_2        | 8         | 4187 * 82   | 128 * 2           | 20.920708       | 37.444523       | 1.074194        | 0               |
| Global_active_power * Sub_metering_2        | 12        | 4187 * 82   | 512 * 8           | 5.351837        | 8.763680        | 1.000724        | 0               |
| Global_active_power * Sub_metering_2        | 16        | 4187 * 82   | 2048 * 32         | 1.633407        | 3.644093        | 1.000139        | 0               |
| Global_active_power * Sub_metering_3        | 8         | 4187 * 33   | 128 * 2           | 4.831763        | 10.784926       | 1.000554        | 3               |
| Global_active_power * Sub_metering_3        | 12        | 4187 * 33   | 512 * 8           | 1.816611        | 2.646363        | 1.000037        | 3               |
| Global_active_power * Sub_metering_3        | 16        | 4187 * 33   | 2048 * 32         | 1.193634        | 1.658485        | 1.000025        | 3               |
| Global_reactive_power * Voltage             | 8         | 533 * 2838  | 8 * 32            | 2.646596        | 23.366835       | 1.004324        | 0               |
| Global_reactive_power * Voltage             | 12        | 533 * 2838  | 32 * 128          | 1.693314        | 11.036392       | 1.000076        | 0               |
| Global_reactive_power * Voltage             | 16        | 533 * 2838  | 128 * 512         | 1.190560        | 3.672114        | 1.000045        | 0               |
| Global_reactive_power * Global_intensity    | 8         | 533 * 222   | 32 * 8            | 2.172512        | 38.585983       | 1.009562        | 0               |
| Global_reactive_power * Global_intensity    | 12        | 533 * 222   | 128 * 32          | 1.324280        | 5.000000        | 1.000649        | 0               |
| Global_reactive_power * Global_intensity    | 16        | 533 * 222   | 512 * 128         | 1.054762        | 1.737829        | 1.000127        | 0               |
| Global_reactive_power * Sub_metering_1      | 8         | 533 * 89    | 32 * 8            | 13.230927       | 110.189728      | 1.000101        | 0               |
| Global_reactive_power * Sub_metering_1      | 12        | 533 * 89    | 128 * 32          | 2.704708        | 9.637048        | 1.000109        | 0               |
| Global_reactive_power * Sub_metering_1      | 16        | 533 * 89    | 512 * 128         | 1.002007        | 1.047304        | 1.000000        | 0               |
| Global_reactive_power * Sub_metering_2      | 8         | 533 * 82    | 32 * 8            | 10.109811       | 100.420036      | 1.000032        | 0               |
| Global_reactive_power * Sub_metering_2      | 12        | 533 * 82    | 128 * 32          | 1.963491        | 8.846316        | 1.000019        | 0               |
| Global_reactive_power * Sub_metering_2      | 16        | 533 * 82    | 512 * 128         | 1.002323        | 1.024087        | 1.000001        | 0               |
| Global_reactive_power * Sub_metering_3      | 8         | 533 * 33    | 64 * 4            | 4.210565        | 36.799061       | 1.002156        | 0               |
| Global_reactive_power * Sub_metering_3      | 12        | 533 * 33    | 256 * 16          | 1.299276        | 2.694490        | 1.000355        | 0               |
| Global_reactive_power * Sub_metering_3      | 16        | 533 * 33    | 1024 * 64         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Voltage * Global_intensity                  | 8         | 2838 * 222  | 64 * 4            | 2.176664        | 11.279375       | 1.000598        | 0               |
| Voltage * Global_intensity                  | 12        | 2838 * 222  | 256 * 16          | 1.359831        | 2.749880        | 1.000367        | 0               |
| Voltage * Global_intensity                  | 16        | 2838 * 222  | 1024 * 64         | 1.058023        | 1.486552        | 1.000211        | 0               |
| Voltage * Sub_metering_1                    | 8         | 2838 * 89   | 64 * 4            | 18.331369       | 21.796688       | 1.000102        | 0               |
| Voltage * Sub_metering_1                    | 12        | 2838 * 89   | 256 * 16          | 4.676184        | 5.398229        | 1.000023        | 0               |
| Voltage * Sub_metering_1                    | 16        | 2838 * 89   | 1024 * 64         | 1.273999        | 1.387039        | 1.000048        | 0               |
| Voltage * Sub_metering_2                    | 8         | 2838 * 82   | 128 * 2           | 22.752365       | 31.776522       | 1.399406        | 0               |
| Voltage * Sub_metering_2                    | 12        | 2838 * 82   | 512 * 8           | 5.919298        | 7.998486        | 1.000025        | 0               |
| Voltage * Sub_metering_2                    | 16        | 2838 * 82   | 2048 * 32         | 1.691759        | 3.895756        | 1.000431        | 0               |
| Voltage * Sub_metering_3                    | 8         | 2838 * 33   | 128 * 2           | 5.284664        | 17.694086       | 1.006569        | 0               |
| Voltage * Sub_metering_3                    | 12        | 2838 * 33   | 512 * 8           | 1.939888        | 17.635786       | 1.000012        | 0               |
| Voltage * Sub_metering_3                    | 16        | 2838 * 33   | 2048 * 32         | 1.129699        | 1.660658        | 1.000181        | 0               |
| Global_intensity * Sub_metering_1           | 8         | 222 * 89    | 32 * 8            | 11.298065       | 19.847881       | 1.001880        | 0               |
| Global_intensity * Sub_metering_1           | 12        | 222 * 89    | 128 * 32          | 2.444978        | 3.263818        | 1.002110        | 0               |
| Global_intensity * Sub_metering_1           | 16        | 222 * 89    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Global_intensity * Sub_metering_2           | 8         | 222 * 82    | 32 * 8            | 5.882402        | 13.243170       | 1.171928        | 0               |
| Global_intensity * Sub_metering_2           | 12        | 222 * 82    | 128 * 32          | 1.650804        | 13.335643       | 1.001266        | 0               |
| Global_intensity * Sub_metering_2           | 16        | 222 * 82    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Global_intensity * Sub_metering_3           | 8         | 222 * 33    | 32 * 8            | 1.914256        | 4.838262        | 1.006103        | 3               |
| Global_intensity * Sub_metering_3           | 12        | 222 * 33    | 128 * 32          | 1.175039        | 1.609076        | 1.000856        | 3               |
| Global_intensity * Sub_metering_3           | 16        | 222 * 33    | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 3               |
| Sub_metering_1 * Sub_metering_2             | 8         | 89 * 82     | 16 * 16           | 13.039970       | 19.639797       | 1.017022        | 0               |
| Sub_metering_1 * Sub_metering_2             | 12        | 89 * 82     | 64 * 64           | 1.514028        | 10.245971       | 1.001174        | 0               |
| Sub_metering_1 * Sub_metering_2             | 16        | 89 * 82     | 256 * 256         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Sub_metering_1 * Sub_metering_3             | 8         | 89 * 33     | 32 * 8            | 3.807171        | 7.145179        | 1.002904        | 0               |
| Sub_metering_1 * Sub_metering_3             | 12        | 89 * 33     | 128 * 32          | 1.136232        | 1.507291        | 1.000011        | 0               |
| Sub_metering_1 * Sub_metering_3             | 16        | 89 * 33     | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |
| Sub_metering_2 * Sub_metering_3             | 8         | 82 * 33     | 32 * 8            | 2.485125        | 5.052917        | 1.003504        | 0               |
| Sub_metering_2 * Sub_metering_3             | 12        | 82 * 33     | 128 * 32          | 1.161939        | 1.606368        | 1.000043        | 0               |
| Sub_metering_2 * Sub_metering_3             | 16        | 82 * 33     | 512 * 128         | 1.000000        | 1.000000        | 1.000000        | 0               |

