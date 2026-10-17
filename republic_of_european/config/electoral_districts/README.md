# 选区配置

每个大区使用 `region/<大区目录名>/` 保存选区映射：

- `Province_Constituencies.csv`：省级选区，以省（Depaertments）和广域市（Metropolitan Citta）为组合基础单位。
- `Canton_Constituencies.csv`：县级选区，以同一省或广域市下的 2–3 个公社为组合基础单位；省区仅为辅助行政区划，不作为选区组成单位。

CSV 表头为 `# name,kind,label,seat_name,population,parent,basis_unit`。组合选区会按基础单位逐行记录，因此同一选区名称可能出现多行；`basis_unit` 使用对应行政单位的稳定 ID。选区使用“1号省选区”“1号县选区”等编号名称。
