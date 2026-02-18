# Project 1: Dataset Description

## Student Exam Performance Dataset

---

## Context

This dataset contains records from a university course tracking student study habits and exam outcomes. Each row represents one student's data for a single exam period. The dataset was collected to study factors that influence student academic performance.

---

## Files

- **`train.csv`** — Training data for model development. This data may contain quality issues that reflect real-world data collection challenges.
- **`test.csv`** — Held-out test data for final model evaluation. Do not use this file during training or preprocessing parameter fitting.

---

## Column Descriptions

| Column Name | Data Type | Description |
|---|---|---|
| `student_id` | Integer | Unique identifier assigned to each student record. |
| `hours_studied` | Float | Self-reported number of hours the student spent studying for the exam. Typical range: 0–15 hours. |
| `sleep_hours` | Float | Average hours of sleep per night during the exam preparation period. Typical range: 3–12 hours. |
| `attendance_rate` | Float | Percentage of lectures attended during the course. Reported as a percentage (e.g., 85.5 means 85.5%). |
| `prev_exam_score` | Float | The student's score on the previous exam in the same course. Scale: 0–100. |
| `lucky_number` | Integer | A number reported by each student in a pre-exam survey. |
| `exam_score` | Float | The student's score on the current exam. Scale: 0–100. **Regression target.** |
| `passed` | Integer (0 or 1) | Whether the student passed the exam (1 = passed, 0 = failed). **Classification target.** |

---

## Target Variables

This dataset supports two prediction tasks:

- **Regression task:** Predict `exam_score` (continuous value, 0–100 scale).
- **Classification task:** Predict `passed` (binary label: 0 or 1).

---

## Important Notes

- The training data may contain data quality issues. Exploring and addressing these issues is an expected part of your workflow.
- Not all columns may be useful as input features for prediction. Consider each column carefully before including it in your model.
- Pay attention to the scale and range of different columns. Features measured on very different scales may require preprocessing.
- The test set is meant for final evaluation only. Any preprocessing parameters (e.g., mean, standard deviation for normalization) should be computed from the training set and then applied to the test set.
