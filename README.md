# AskMe

People ask questions and others answers them

## Features
  
  - Login/Sign up/Sign Out
  - Files-based system
  - Multiple users can use it in same time (parallel)
  - System support threads questions
 
## User Operations
  
  - Print questions: From you, To you, Feed Questions
  - Ask and answer questions
  - Delete a question (which might be a thread)
  
## Design

-   #### Every user and question has a system generated ID


-    #### A thread question
     - If a question is answered. Others can ask questions in the original question thread
     - Original question is parent for all of them
     - If this parent question is removed, the whole thread is removed

-    #### Correctness
     - You can’t do something wrong!
     - You can’t remove a question ask to someone else!
     - You only see/edit the details of your question
 
 
 ## Run Sample
 ![askme](https://user-images.githubusercontent.com/43843461/206220768-150e2c12-c763-4082-b164-8b28ed7db620.png)
