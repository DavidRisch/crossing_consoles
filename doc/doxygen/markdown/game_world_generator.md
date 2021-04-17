\page game-world-generator World Generation

# World Generation

The class `RandomWorldGenerator` creates a world in two steps:

* First, the landscape is generated using Perlin Noise.
* After that, buildings are created randomly (if the world is big enough).

## Generation of the landscape

### Perlin Noise

Reference: https://web.archive.org/web/20160530124230/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

#### Noise functions

> *A noise function is essentially a seeded random number generator. It takes an integer as a parameter, and returns a  
> random number based on that parameter. If you pass it the same parameter twice, it produces the same number twice.  
> It is very important that it behaves in this way, otherwise the Perlin function will simply produce nonsense.*

#### Results in 2D

Multiple noise functions with different parameters (top) and final result (bottom):
<div>
    <img style = "float: left; margin: 2px;" src = "perlin_a.jpg" />
    <img style = "float: left; margin: 2px;" src = "perlin_b.jpg" />
    <img style = "float: left; margin: 2px;" src = "perlin_c.jpg" />
    <img style = "float: left; margin: 2px;" src = "perlin_d.jpg" />
    <img style = "float: left; margin: 2px;" src = "perlin_e.jpg" />
    <img style = "float: left; margin: 2px;" src = "perlin_f.jpg" />
</div>

<img style = "clear: both; display: block; margin-left: 331px;" src = "perlin_result.jpg" />

### Creating the landscape

The Perlin noise produces a value for each coordinate pair (`x`, `y`) that can be interpreted as the height (`z`).  
The height is used by the class `HeightMap` to determine what block type to use.

## Generation of simple buildings

The number of buildings generated depends on the size of the world:  
While the landscape is being generated, all the occurring empty blocks are counted.  
The number of buildings is calculated by dividing the remaining area of open space by the maximum area of a building.  
The result is multiplied by the specified `building_rate` (`0.0` - `1.0`).

To generate buildings, rectangles between a minimum and maximum size are randomly created and placed in the existing
world.  
\image html building_rectangle.png   
Afterwards they are hollowed out by placing a second rectangle (`size - 2`)
consisting of empty blocks at the relative position (1, 1).  
\image html building_hollow.png   
Finally, doors are created randomly by removing blocks along the edges of the building.  
\image html building_doors.png
