Level = {
  assets = {
    [0] =
    { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png"},
    { type = "texture", id = "helicopter-texture", file = "./assets/images/chopper-spritesheet.png"},
    { type = "texture", id = "takeoff-base-texture", file = "./assets/images/takeoff-base.png"},
    { type = "texture", id = "tank-texture", file = "./assets/images/tank-tiger-right.png"},
    { type = "texture", id = "truck-texture", file = "./assets/images/truck-ford-left.png"},
    { type = "texture", id = "radar-texture", file = "./assets/images/radar.png"},
    { type = "texture", id = "bullet-texture", file = "./assets/images/bullet.png"},
    { type = "font", id = "arial-font", file = "./assets/fonts/arial.ttf", font_size = 16},
    { type = "font", id = "charriot-font", file = "./assets/fonts/charriot.ttf", font_size = 16}
  },

  tilemap = {
      map_file = "./assets/tilemaps/jungle.map",
      texture_asset_id = "tilemap-texture",
      num_rows = 20,
      num_cols = 25,
      tile_size = 32,
      tile_scale = 2.0
  },

  entities = {
    [0] = {
      tag = "player",

      components = {
        transform = {
          position = { x = 240, y = 300 },
          scale = {x = 2.0, y = 2.0 },
          rotation = 0 -- degrees
        },

        rigid_body = {
          velocity = { x = 0, y = 0 }
        },

        sprite = {
          texture_asset_id = "helicopter-texture",
          width = 32,
          height = 32,
          z_index = 3
        },

        animation = {
          num_frames = 2,
          speed_rate = 10 -- fps
        },

        box_collider = {
          offset = { x = 0, y = 0},
          width = 32,
          height = 32
        },

        health = {
          health_percentage = 100
        },

        projectile_emitter = {
          projectile_velocity = { x = 250, y = 0 },
          projectile_duration = 10000, -- ms
          repeat_frequency = 0, -- ms
          damage = 10,
          is_friendly = true
        },

        keyboard_control = {
          up_velocity = { x = 0, y = -240 },
          right_velocity = { x = 240, y = 0},
          down_velocity = { x = 0, y = 240 },
          left_velocity = { x = -240, y = 0 }
        },

        camera_follow = {
          will_follow = true
        },

        godmode_component = {
          active = false
        },

      },
    },

    {
      -- tank
      group = "enemies",

      components = {
        transform = {
          position = { x = 240, y = 600 },
          scale = {x = 2.0, y = 2.0 },
          rotation = 0
        },

        rigid_body = {
          velocity = { x = 50, y = 0 }
        },

        sprite = {
          texture_asset_id = "tank-texture",
          width = 32,
          height = 32,
          z_index = 2
        },

        box_collider = {
          offset = { x = 0, y = 0},
          width = 32,
          height = 32
        },

        health = {
          health_percentage = 100
        },

        projectile_emitter = {
          projectile_velocity = { x = 250, y = 0 },
          projectile_duration = 5, -- seconds
          repeat_frequency = 2, -- seconds
          damage = 10,
          is_friendly = false
        },

        godmode_component = {
          active = false
        },

      },
    },
    -- add others under here
  }


}
