# 官员与议会人员名册

本目录按 `administrative_districts_positions` 的职位模板建立人员映射，不推定未明确规定的席位数量或真实任命结果。

- `nation.csv`：联邦层级职位。
- `nation/member_states.csv`：加盟州层级职位。

所有人员 ID、姓名及 `home_commune_id` 直接引用欧罗巴共和国公民名册 `republic_of_european/config/politicians/region/*.csv`，不另造 HFR 公民或姓名。因其他加盟州暂无各自的公民名册，跨州职位也沿用该来源；`home_commune_id` 保留原来源地，不表示该人员属于职位所列加盟州。每个职位模板配置一名不同的样本人员，人员 ID 在本目录内不重复。

CSV 表头为 `# position_id,position_name,basis_unit,person_id,person_name,home_commune_id`；职位 ID 直接取职位配置括号内的稳定 ID。
