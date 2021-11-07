| Progetto  | Nome e Cognome     | Data di consegna |
|:---------:|:------------------:|:----------------:|
| WordCount | Giuseppe Di Palma  |    xx/xx/2021    |

Eseguire il tutto su M4.XLARGE(4CPU & 16GB Ram)

***Problem Description***

The word count is the number of words in a document or passage of text. Word counting may be needed when a text is required to stay within specific numbers of words. This may particularly be the case in academia, legal proceedings, journalism, and advertising.

Word count is commonly used by translators to determine the price of a translation job. Word counts may also be used to calculate measures of readability and to measure typing and reading speeds (usually in words per minute). When converting character counts to words, a measure of 5 or 6 characters to a word is generally used for English

We will be doing a version of map-reduce using MPI to perform word counting over a large number of files

There are three steps for this process:

- the MASTER node reads the file list (or a directory), which will contain the names of all the files that are to be counted. Note that only 1 of your processes should read the files list. Then each of the processes should receive their portion of the file from the MASTER process. Once a process has received its list of files to process, it should then read in each of the files and perform a word counting, keeping track of the frequency each word found in the files occurs. We will call the histogram produced the local histogram. This is similar to the map stage or map-reduce.
- the second phase is combining the frequencies of words across processes. For example, the word 'cat' might be counted in multiple processes, and we need to add up all these occurrences. This is similar to the reduced stage of map-reduce.
- the last phase is to have each of the processes send their local histograms to the master process. The MASTER process just needs to gather up all this information. Note that there will be duplicate words between processes. The master should create a CSV formatted file with the words and frequencies ordered.

**NOTES.** The hard part of the problem concerns splitting the computation among the processors. For instance, if we split the files between processors, we cannot have a good partitioning scheme because some files must be bigger and bigger than other files. A good partitioning scheme must consider the number of words in each file, and split according to this value.

## What does the project is about?

The final exam of this part of the course consists of realizing a programming assignment and evaluating its performance on an Amazon EC2 instances cluster.  

Each student must develop a solution for an assigned homework using a parallel programming approach, and by exploiting distributed memory. The solution must be written in the C programming language, exploiting the OpenMPI library. Furthermore, the developed solution must be experimented on a homogeneous cluster running on top of the Amazon Web Services cloud computing platform.

The cluster is composed of several supported [AWS Educate Instances](https://s3.amazonaws.com/awseducate-starter-account-services/AWS_Educate_Starter_Accounts_and_AWS_Services.pdf). The obtained results must be presented in terms of [strong and weak scalability](https://www.sharcnet.ca/help/index.php/Measuring_Parallel_Scaling_Performance), varying the number of computing processors from *1* to *number-of-vcp* *x* *number-of-instances*.

> For example, if we run a cluster machine of *4* t2.large (2 VCPU) nodes, we have to perform the scalability of our solution for P={1,2,4,5,6,7,8}.

The total number of processors is equal to the number of Virtual CPUs on the running instances. The benchmark must exploit **8** instances for the more big experiment. The student must describe the solution and the benchmark in a report file, written in Markdown, and included in the submission in both the format Markdown and PDF. The report file must also describe the compilation phase and how to reproduce the results obtained, considering the Docker MPI environment of the course.

## Project milestones

A correct solution must generate the same results independently by the number of processors used.

The program should provide simple functionalities for testing the results, allowing us to quickly compare the results of the execution using *1* processor and *P* processors.

It is crucial keeping your solution *clear, concise, and easily understandable* by providing good code quality and fully describing it in the attached document.

## Structure of your project

- A single source C file of your project' solution.
- A PDF report document, including images and formatted code (only meaningful part of your solution), structure example:
    - Introduction
    - Brief description of your solution
    - Implementation details
    - Execution instructions
    - Correctness discussion
    - Benchmarks
