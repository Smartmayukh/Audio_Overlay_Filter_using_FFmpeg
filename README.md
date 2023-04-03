# Audio_Overlay_Filter_using_FFmpeg

Copyright (c) 2023 Mayukhmali Das

This repository is an attempt to develop an efficient Audio Overlay Filter using the FFmpeg library and system function in C, which will enable users to mix samples in a stream with other samples at any point in the stream as specified by the user. The filter will provide users with several options, including Position, Loop_Time, Gain_of_Base, and Gain_of_Overlay. It will also include auxiliary features such as fade in, fade out, crossfade, time stretch, and best sync point. 

## User Options

I plan to introduce the following options for the users

<pre>
 Position 
 Loop_Time
 Gain_of_Base
 Gain_of_Overlay 
 Silent
</pre>

## Auxiliary features 

<pre>
Fade in Fade out options for the Overlay
Crossfade
Time stretch
Best Sync Point
</pre>

# Explanation of the features : 

## Main features 

<pre>
Position : This will be a number in  milliseconds where the user wants to insert the overlay file.

Loop_Time :  Number of times the overlay will be used. If the net length of overlay  exceeds the base audio  length, the overlay will be trimmed to match the base audio length.

Gain_of_Base : Gain of the base audio.

Gain_of_Overlay: Gain of the overlay audio.

Silent : Make base audio silent while overlaying.
</pre>

## Auxiliary features that can be added : 

### Fade in Fade out:  

FFmpeg's afade filter is capable of applying fade-in and fade-out effects to input audio streams. Additionally, it offers several options to modify the volume of the audio during the fade-in effect as shown below. We can incorporate this afade filter to give a fading in/out effect to our overlay filter.

![image](https://user-images.githubusercontent.com/64318469/229578832-a7682974-250e-4da4-95bc-aff9716a7a70.png)



### Crossfade : 

If we are using the silent option to mute the base audio while overlaying we can add a crossfade for a smoother transition from the base audio to the overlaying audio. FFmpeg has a acrossfade filter. We can also create our own filter. 

We can use a technique called Optimal Tempo Adjustment which automatically generates a smooth Song to Song transition


![image](https://user-images.githubusercontent.com/64318469/229578769-e27464b6-680f-4618-9257-a359aa7abc84.png)



This algorithm of a Mixing System with Optimal  Tempo Adjustment is given in the following paper
 
https://ismir2009.ismir.net/proceedings/PS1-14.pdf  

Also, when two songs are played back-to-back and the strong beats are adjusted to match the weaker beat of the other song, it can create a jarring listening experience. To avoid this problem, a cross-fade technique is proposed in the above paper, which involves computing the cross-correlation of the beats of the songs within the crossfade range. This helps to seamlessly blend the two songs together, without causing any discomfort to the listener.


### Time stretching: 

Time stretching refers to the technique of altering the speed or length of an audio signal, while maintaining its original pitch. We can incorporate FFmpeg atempo filter for this purpose. 

### Best Sync Point: 

We can mix the two audio files, the base and the overlay, based on their beats. To achieve this, we can utilize a function that determines the optimal synchronization point between the two audio files before mixing them. This function will take the beat samples of both the top and bottom files as well as the maximum sample to be mixed.

To calculate the best synchronization point, the function will slide the beat samples of the overlay file over the beat samples of the base file, while measuring the number of matches between the two files within a given offset.

The algorithm will be like this. First we will initialize a variable called "best_sync_point" variable to zero, which represents the most favorable synchronization point found so far. We will then iterate through each possible synchronization point in the received signal, calculating the correlation coefficient between the reference signal and the received signal at that point. The correlation coefficient determines the similarity between the two signals at a particular time offset.

In summary, this algorithm functions by comparing the correlation coefficient between the reference signal and the received signal at every possible synchronization point. It then selects the synchronization point that maximizes the correlation coefficient as the optimal synchronization point.


## Flowchart for the C implementation of the overlay Filter

![image](https://user-images.githubusercontent.com/64318469/229578950-1570cf94-e791-40ad-b87d-f85f3461c536.png)











