# space-shape

**Shape Your Information**

``` mermaid
graph LR
m1{Map} -->|a| m1d1(Data)
m1{Map} -->|b| m1d2(Data)
m1{Map} -->|c| m1m2{Map}
m1m2{Map} -->|a| m2l1{List}
m1m2{Map} -->|b| m2d1(Data)
m2l1{List} -->|1| m2l1d1(Data)
m2l1{List} -->|2| m2l1d2(Data)
m2l1{List} -->|3| m2l1d3(Data)
m2l1{List} -->|4| m2l1d4(Data)
```
