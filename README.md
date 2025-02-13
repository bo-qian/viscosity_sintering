<!--
 * @Author: Bo Qian
 * @Date: 2025-02-11 17:10:06
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-12 15:22:53
 * @LastEditors: Bo Qian
 * @Description: Readme
 * @FilePath: /viscosity_sintering/README.md
-->
viscosity_sintering
=====

Fork "viscosity_sintering" to create a new MOOSE-based application.

For more information see: [https://mooseframework.org/getting_started/new_users.html#create-an-app](https://mooseframework.org/getting_started/new_users.html#create-an-app)

Debug code by using valgrind, 使用 `tee` 同时输出到终端和文件：

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./viscosity_sintering-opt -i viscosity_sintering_IC_2D.i 2>&1 | tee valgrind_output.txt
```

- `tee valgrind_output.txt` 会将输出写入到 `valgrind_output.txt` 文件，同时还会将它显示在终端上。