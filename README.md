Para generar nuevos datasets se necesita de:
- Un world file, creado manualmente con la estructura deseada y la superficie de suelo de interés.
  - Cuando se guarda el world, tener cuidado porque la posicion de los modelos no se guarda. La posicion se almacena en <state><model> pero no hace nada, es necesario poner esa posicion en <world><model>
- Un archivo de configuración en formato yaml, que indique los parámetros de generación del dataset.
- Este world file ha de incluir el plugin de generación de datasets.

    <plugin name='arvc_gazebo_ros_test_generator' filename='libarvc_gazebo_ros_test_generator.so'>
      <yaml_config><RUTA_ABSOLUTA_AL_ARCHIVO_DE_CONFIGURACION_DESEADO_SIN_COMILLAS_NI_ESPACIOS></yaml_config>
    </plugin>