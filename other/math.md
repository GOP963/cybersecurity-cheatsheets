
## **فاکتوریل (Factorial)**

فاکتوریل در ریاضی یعنی:

n!=n×(n−1)×(n−2)×...×1

برای مثال:

- 3! = 3 × 2 × 1 = 6
    
- 5! = 5 × 4 × 3 × 2 × 1 = 120


```
int factorial(int n)
{
    if (n <= 1) return 1;
    else return n * factorial(n - 1);
}
```
