import matplotlib.pyplot as plt
import pandas as pd

ExampleOne = "OilGuard_DataCollector_Example[2].xlsx"

RawData1 = pd.read_excel(ExampleOne)

Time_Data = RawData1.iloc[:,1]
Temperature_Data = RawData1.iloc[:,2]
Oil_Level_Data = RawData1.iloc[:,3]
Viscosity_Data = RawData1.iloc[:,4]

Time_Data = Time_Data.astype(str)

Temperature_Range = list(range(15,30,2))
fig, axs = plt.subplots(1,2)

axs[0].plot(Time_Data, Temperature_Data)
axs[0].set_title('Temperature vs time')
axs[0].set_xlabel("Time")
axs[0].set_ylabel("Temperature Readings")
axs[0].set_yticks(Temperature_Range)
axs[0].set_xticks([])
axs[0].grid()

axs[1].plot(Oil_Level_Data)
axs[1].set_title('Oil Level vs time')
axs[1].set_xlabel("Time")
axs[1].set_ylabel("Oil Level Readings")
axs[1].set_ylim(0,11)
axs[1].set_xticks([])
axs[1].grid()

plt.tight_layout()

plt.show()



# plt.scatter(x_data, y_data) --> this function scatter points in x-y plane
#  # c="" --> attribute for color
# s= --> attrinut for line size
# marker="" --> attribute for shape of point marker : square, star

#plt.plot() --> take x data and y data or only y data and draw them as a line
# c="" --> attribute for color
# s= --> attrinut for line size