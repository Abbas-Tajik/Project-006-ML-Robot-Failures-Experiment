<h2 align="center">🤖 Robot Execution Failures – End-to-End Machine Learning Analysis & Experiment</h2> <p align="center"> <img src="https://img.shields.io/badge/Category-Machine%20Learning-555" /> <img src="https://img.shields.io/badge/Techniques-Ensemble%20Methods-555" /> <img src="https://img.shields.io/badge/Skills-Data%20Preprocessing%20%7C%20Robotics-555" /> <img src="https://img.shields.io/badge/Languages-Python%20%7C%20C++-555" /> </p> <p> This project explores how machine learning can be used to identify and classify robot execution failures using force and torque sensor data. It forms the final project of my Level 5 AI & Machine Learning bootcamp and is designed as a clear, end-to-end example of a full ML workflow: data loading, exploration, preprocessing, modelling, evaluation, and interpretation.

The aim was to build something practical, transparent, and easy for other developers to follow, whilst showing how sensor data can support early detection of mechanical issues in robotic systems.

</p> <p> I used the Robot Execution Failures dataset from the UCI Machine Learning Repository. It contains 463 samples, each representing 16 possible robot failure types, with 90 features derived from force/torque readings at 15 time steps.

No personal or sensitive information is included. All analysis is strictly technical and focused on system behaviour.

</p> <p> Tools: Python, Pandas, NumPy, scikit-learn, XGBoost, Matplotlib, Seaborn. Workflow: Parsing the dataset into a usable structure, quality checks (duplicates, imbalance), exploratory analysis, preprocessing, model training, and evaluation with metrics like accuracy, precision, recall, and F1-score. </p> <p> Ten algorithms were tested, including Logistic Regression, Decision Trees, Naïve Bayes, SVM (linear and RBF), k-NN, Random Forest, Gradient Boosting, XGBoost, MLP, and a Stacking Classifier. The Stacking Classifier achieved the strongest overall performance at around 69% accuracy. </p> <p> Frequent failure classes were predicted reliably, whilst rare classes remained challenging due to limited representation. Feature importance indicated that early-stage sensor readings contributed most to predictive performance. </p> <p> I also created a small Arduino-based demonstration using my robot arm to illustrate several of the failure patterns seen in the dataset. This includes obstructions, collisions, and servo stall behaviour. The demo code is included as AT_RobotArmExp_V11.ino. </p> <p> Limitations include dataset imbalance, small sample size, and difficulties with under-represented classes. The project is a proof of concept; further data and calibration would be required for production deployment. </p> <p> Files included: full notebook (AT_RobotFailures_V9.ipynb), demonstration code, supporting README, original dataset, and planning document. </p> <p> This project reinforced the importance of strong exploratory analysis, appropriate preprocessing, and careful evaluation when working with imbalanced multi-class problems. It also highlighted the benefits of ensemble methods for stability and reliability across complex datasets. </p>

</p>

<hr/>
<p align="center">
  <strong>Please click below to see my 3D-printed predictive maintenance robot arm – end-to-end machine learning experiment!</strong><br>
</p>
  
<p align="center">
  <a href="https://www.youtube.com/watch?v=A1FaLFZFPcU">
    <img src="https://img.youtube.com/vi/A1FaLFZFPcU/0.jpg" alt="Video Thumbnail">
  </a>
</p>

<hr/>

<h3>Acknowledgements</h3>

<p>
  This project is based on the Robot Execution Failures dataset from the UCI Machine Learning Repository 
  (Lopes, L. S., & Camarinha-Matos, L. M., 1999). The dataset served as the foundation for exploring 
  feature relationships, model behaviour, and experimental evaluation across 16 failure types using 
  force and torque sensor data.
</p>

<p>
  The implementation utilises scikit-learn, pandas, NumPy, Matplotlib, Seaborn, and XGBoost, 
  following standard machine learning workflows for classification tasks. The robot arm demonstration 
  (AT_RobotArmExp_V11) was developed to physically illustrate the failure patterns identified through 
  the ML analysis.
</p>

<p>
  Robot design: EEZYbotARM MK2 by daGHIZmo  
  (<a href="https://www.thingiverse.com/thing:1454048">https://www.thingiverse.com/thing:1454048</a>)
</p>

<p>
  This project was completed as part of a Level 5 AI and Machine Learning Bootcamp, with the goal 
  of creating a learning resource that demonstrates end-to-end machine learning workflows for 
  developers new to the field.
</p>
