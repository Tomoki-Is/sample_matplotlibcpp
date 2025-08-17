#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "config.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
    const Config config("sample_matplotlibcpp.conf", {});

    std::vector<double> x, y;

    // CSV読み込み
    std::ifstream file(config.getString("trajectory_path"));
    if (!file.is_open())
    {
        std::cerr << "Failed to open csv" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string xs, ys;
        if (std::getline(ss, xs, ',') && std::getline(ss, ys))
        {
            x.push_back(std::stod(xs));
            y.push_back(std::stod(ys));
        }
    }
    file.close();
    plt::named_plot("data", x, y, "o-");

    // Python側のインタープリタ取得
    auto &interp = plt::detail::_interpreter::get();

    // C++のベクタをPythonのlistに変換
    PyObject *pyx = PyList_New(x.size());
    PyObject *pyy = PyList_New(y.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        PyList_SetItem(pyx, i, PyFloat_FromDouble(x[i]));
        PyList_SetItem(pyy, i, PyFloat_FromDouble(y[i]));
    }

    // __main__ のdictに登録
    PyObject *main_module = PyImport_AddModule("__main__");
    PyObject *global_dict = PyModule_GetDict(main_module);
    PyDict_SetItemString(global_dict, "x", pyx);
    PyDict_SetItemString(global_dict, "y", pyy);

    // Pythonコードを実行
    PyRun_SimpleString(R"(
import matplotlib.pyplot as plt
import numpy as np

points = np.column_stack((x, y))
fig = plt.gcf()
ax = plt.gca()

# 1つだけマーカーを保持する
(marker,) = ax.plot([], [], "ro")

def onclick(event):
    if event.xdata is None or event.ydata is None:
        return
    click = np.array([event.xdata, event.ydata])
    dists = np.linalg.norm(points - click, axis=1)
    idx = np.argmin(dists)
    # マーカーの位置を更新
    marker.set_data(points[idx,0], points[idx,1])
    print("clicked:", points[idx])
    fig.canvas.draw()

fig.canvas.mpl_connect("button_press_event", onclick)
)");

    plt::show();
}
