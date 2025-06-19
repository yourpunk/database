# 🧠 Parallel Predicate Query Evaluator

A high-performance system for evaluating compound logical predicates (`AND` / `OR`) over large simulated datasets — with a focus on **parallelism**, **early cancellation**, and **query planning logic**.

Think: *"What if a baby database could still think smart and run fast?"*

---

## 🔍 What This Is

This project simulates a predicate evaluation engine (like `WHERE` clauses in SQL), comparing:

- ✅ `is_satisfied_for_all(predicates, table)`
- ✅ `is_satisfied_for_any(predicates, table)`

Each predicate is artificially slowed down (2μs delay) to simulate I/O latency, testing how well the system performs **under pressure**.

Built-in **OpenMP parallelism** is used to:
- Execute independent checks across rows
- Cancel early if a result is known (`OMP_CANCELLATION` required)

---

## 🧠 Why It Matters

Most naive query evaluators brute-force checks across all data.  
This project demonstrates:

- ⚙️ Smart use of **OpenMP parallelism** with cancellation
- 🧩 Clean **predicate logic abstraction** (`std::function`-style)
- 🔎 Simulated stress-testing through synthetic I/O latency
- ✅ Reproducible benchmarking & logic verification

> 🔥 Designed to **think like a database**, not just loop like a script

---

## 🧪 Test Cases

| Test Name         | Purpose                            |
|-------------------|-------------------------------------|
| `TestAll<true>`   | Validates complete `AND` pass       |
| `TestAny<true>`   | Validates partial `OR` pass         |
| `TestAll<false>`  | Ensures `AND` short-circuits safely |
| `TestAny<false>`  | Ensures `OR` doesn't false-positive |

Example output:

```yaml
true = is_satisfied_for_all(...)   132ms
true = is_satisfied_for_any(...)    58ms
false = is_satisfied_for_all(...)   --- wrong result ---
false = is_satisfied_for_any(...)   23ms
```
---

## 🧩 File Structure

|File|	Description|
|----|-------|
|`query.h` |	⭐ My core logic: predicate composition & evaluation engine|
|`generator.*` |	Synthetic data & predicate creation|
|`tests.h` |	Wraps logic into repeatable test harnesses|
|`main.cpp`	| Runs test matrix, logs times, verifies output|
|`params.h`	| Tweak dataset size, delay time, etc.|

---

## ⚙️ How to Run

```bash
g++ -std=c++17 -fopenmp main.cpp generator.cpp -o query_eval
OMP_CANCELLATION=true ./query_eval
```
> Requires **OpenMP** with cancellation support.<br>If your compiler warns you, double-check `OMP_CANCELLATION` is set.

---

## ✍️ My Role

- Designed and implemented all logic inside query.h
- Refactored original "framework" code for modularity and clean benchmarking
- Introduced OpenMP cancellation to avoid wasteful cycles

---

## 💬 What I Learned

> Writing efficient query logic is about when to stop, not just how to start.
This project taught me how to:
- Think like a query planner
- Structure parallel logic around short-circuiting
- Test correctness under multithreaded constraints

---

## 📜 License

**MIT**. Use it, fork it, break it. Just don’t ship it without understanding it.

---

## 👤 Author

🦾 Crafted by Aleksandra Kenig (aka [yourpunk](https://github.com/yourpunk)).
### 💌 Feedback? Ideas? Collab offers? I’m all ears.
