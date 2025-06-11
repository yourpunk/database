# 🧠 Parallel Query Evaluator

This project benchmarks the evaluation of logical queries (AND / OR) over simulated data using parallelism.  
It's built like a minimal fake database engine — just enough logic to stress-test your brain and your CPU.

> ✍️ I wrote `query.h` myself — it contains the logic for evaluating `is_satisfied_for_all` and `is_satisfied_for_any`  
> The rest is just framework code from school. I made it work.

## 💡 What it actually does

There are two types of queries:

- `is_satisfied_for_all(predicates, table)`  
  → True if every predicate matches **at least one** row

- `is_satisfied_for_any(predicates, table)`  
  → True if **any** predicate matches at least one row

Each predicate simulates a slow check (e.g., disk access) using an artificial 2-microsecond delay.

The whole thing runs in parallel with OpenMP to see how fast we can crunch large queries.


## 🧪 Tests included

| Test Case                            | Description                                |
|-------------------------------------|--------------------------------------------|
| `TestAll<true>`                     | All predicates must be satisfied           |
| `TestAny<true>`                     | At least one predicate must be satisfied   |
| `TestAll<false>`, `TestAny<false>` | Negative tests to catch false positives    |

Output example:
```yaml
true = is_satisfied_for_all(...) 132ms
true = is_satisfied_for_any(...) 58ms

false = is_satisfied_for_all(...) --- wrong result ---
false = is_satisfied_for_any(...) 23ms
```

## 🔧 How to run it

```bash
g++ -std=c++17 -fopenmp main.cpp generator.cpp -o query_eval
./query_eval
```
> ⚠️ OpenMP cancellation must be enabled. <br><br>If you see a warning, run with: `OMP_CANCELLATION=true ./query_eval`

## 🧩 File Breakdown

|File |	What it does|
|-----|-------------|
|query.h	| ⭐ My work. Core logic for queries.|
|generator.* |	Generates random data and predicates.|
|tests.h	| Test wrappers for true/false logic.|
|main.cpp	| Runs the whole thing and times it.|
|params.h	| Tweak query length, row count, etc.|

## 🧠 Why I care

Most students just blindly copy logic or write “if/else” spaghetti. I wanted to actually understand what makes a predicate system tick, how to write clean functional logic, and how to test it under pressure.

I also wanted to try cancellation with `OpenMP` — because not everything should be brute-forced till the end.

## 📜 License
MIT. Copy, fork, break it, improve it — just don’t ship nonsense.

## 👤 Author
🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk)).<br>
💌 Wanna collab or throw some feedback? You know where to find me.
